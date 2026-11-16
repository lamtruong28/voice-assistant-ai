#include "ws_transport.h"
#include "app_config.h"
#include <WiFi.h>

bool WsTransport::begin()
{
    // rxRingBuf_ = xRingbufferCreate(Config::RX_RINGBUF_BYTES, RINGBUF_TYPE_BYTEBUF);
    // txRingBuf_ = xRingbufferCreate(Config::TX_RINGBUF_BYTES, RINGBUF_TYPE_BYTEBUF);
    // Keep fast 16KB mic buffer in internal SRAM
    // rxRingBuf_ = xRingbufferCreate(Config::RX_RINGBUF_BYTES, RINGBUF_TYPE_BYTEBUF);

    // // Allocate larger 48KB (or even 96KB) speaker buffer in 2MB QSPI PSRAM
    // txRingBuf_ = xRingbufferCreateWithCaps(
    //     Config::TX_RINGBUF_BYTES,
    //     RINGBUF_TYPE_BYTEBUF,
    //     MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    // if (!rxRingBuf_ || !txRingBuf_)
    // {
    //     Serial.println("[NET] FATAL: Failed to allocate RingBuffers in SRAM");
    //     return false;
    // }

    // 1. Keep fast 16KB mic buffer in internal SRAM
    rxRingBuf_ = xRingbufferCreate(Config::RX_RINGBUF_BYTES, RINGBUF_TYPE_BYTEBUF);

    // 2. Allocate large 96KB speaker buffer in 2MB QSPI PSRAM
    if (psramFound())
    {
        txRingBuf_ = xRingbufferCreateWithCaps(
            Config::TX_RINGBUF_BYTES,
            RINGBUF_TYPE_BYTEBUF,
            MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        Serial.printf("[MEM] PSRAM detected (%lu bytes). Speaker buffer allocated in PSRAM.\n", (unsigned long)ESP.getPsramSize());
    }
    else
    {
        // Fallback to smaller 48KB buffer in internal SRAM if PSRAM is unavailable
        txRingBuf_ = xRingbufferCreate(48 * 1024, RINGBUF_TYPE_BYTEBUF);
        Serial.println("[MEM] PSRAM not found. Fallback to 48KB Internal SRAM.");
    }

    if (!rxRingBuf_ || !txRingBuf_)
    {
        Serial.println("[NET] FATAL: Failed to allocate RingBuffers!");
        return false;
    }

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); // Disable modem sleep for zero-jitter WebSocket audio streaming
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_PASS);
    Serial.print("[WIFI] Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
        Serial.print(".");
    }
    Serial.printf("\n[WIFI] Connected! IP: %s\n", WiFi.localIP().toString().c_str());

    ws_.begin(Config::WS_HOST, Config::WS_PORT, Config::WS_PATH);
    ws_.onEvent([this](WStype_t type, uint8_t *payload, size_t length)
                { this->handleWsEvent(type, payload, length); });
    ws_.setReconnectInterval(Config::WS_RECONNECT_MS);

    return true;
}

void WsTransport::loop()
{
    ws_.loop();
    if (currentState_ == AssistantState::LISTENING && ws_.isConnected())
    {
        drainMicBufferToServer();
    }
}

void WsTransport::setState(AssistantState newState)
{
    currentState_ = newState;
}

void WsTransport::triggerStartListening()
{
    if (!ws_.isConnected())
    {
        Serial.println("[WS] Cannot record: Server disconnected");
        return;
    }
    flushBuffers();
    ttsFinished_ = false;
    currentState_ = AssistantState::LISTENING;
    ws_.sendTXT("start");
    Serial.println("[STATE] -> LISTENING");
}

void WsTransport::triggerStopListening()
{
    if (currentState_ != AssistantState::LISTENING)
        return;
    currentState_ = AssistantState::PROCESSING;
    ws_.sendTXT("stop");
    Serial.println("[STATE] -> PROCESSING");
}

bool WsTransport::pushMicAudio(const uint8_t *data, size_t length)
{
    return xRingbufferSend(rxRingBuf_, data, length, 0) == pdTRUE;
}

uint8_t *WsTransport::popSpeakerAudio(size_t *outSize, size_t maxBytes, TickType_t waitTicks)
{
    return static_cast<uint8_t *>(
        xRingbufferReceiveUpTo(txRingBuf_, outSize, waitTicks, maxBytes));
}

void WsTransport::releaseSpeakerAudio(uint8_t *item)
{
    vRingbufferReturnItem(txRingBuf_, item);
}

void WsTransport::flushBuffers()
{
    size_t itemSize = 0;
    void *data = nullptr;

    while ((data = xRingbufferReceiveUpTo(rxRingBuf_, &itemSize, 0, 4096)) != nullptr)
    {
        vRingbufferReturnItem(rxRingBuf_, data);
    }
    while ((data = xRingbufferReceiveUpTo(txRingBuf_, &itemSize, 0, 4096)) != nullptr)
    {
        vRingbufferReturnItem(txRingBuf_, data);
    }
}

void WsTransport::drainMicBufferToServer()
{
    size_t itemSize = 0;
    uint8_t *chunk = static_cast<uint8_t *>(
        xRingbufferReceiveUpTo(rxRingBuf_, &itemSize, 0, Config::MIC_CHUNK_SAMPLES * sizeof(int16_t)));

    if (chunk != nullptr && itemSize > 0)
    {
        ws_.sendBIN(chunk, itemSize);
        vRingbufferReturnItem(rxRingBuf_, chunk);
    }
}

void WsTransport::handleWsEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        Serial.println("[WS] Connected to backend");
        currentState_ = AssistantState::IDLE;
        break;

    case WStype_DISCONNECTED:
        Serial.println("[WS] Disconnected from backend");
        currentState_ = AssistantState::IDLE;
        flushBuffers();
        break;

    case WStype_TEXT:
    {
        String msg = reinterpret_cast<char *>(payload);
        msg.trim();
        if (msg == "tts:start")
        {
            flushBuffers();
            ttsFinished_ = false;
            currentState_ = AssistantState::SPEAKING;
            Serial.println("[STATE] -> SPEAKING");
        }
        else if (msg == "tts:end")
        {
            ttsFinished_ = true;
        }
        break;
    }

    case WStype_BIN:
        if (currentState_ == AssistantState::SPEAKING && length > 0)
        {
            if (xRingbufferSend(txRingBuf_, payload, length, pdMS_TO_TICKS(50)) != pdTRUE)
            {
                Serial.println("[WARN] Speaker RingBuffer full, dropping packet");
            }
        }
        break;

    default:
        break;
    }
}