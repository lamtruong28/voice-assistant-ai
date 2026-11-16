#pragma once

#include <Arduino.h>
#include <WebSocketsClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include "app_state.h"

/**
 * @brief Manages WiFi connectivity, WebSocket client, state transitions, and FreeRTOS RingBuffers.
 */
class WsTransport
{
public:
    bool begin();
    void loop();

    AssistantState getState() const { return currentState_; }
    void setState(AssistantState newState);

    bool isTtsFinished() const { return ttsFinished_; }
    void clearTtsFinished() { ttsFinished_ = false; }

    // State transition triggers
    void triggerStartListening();
    void triggerStopListening();

    // RingBuffer accessors for Core 1 Audio Task
    bool pushMicAudio(const uint8_t *data, size_t length);
    uint8_t *popSpeakerAudio(size_t *outSize, size_t maxBytes, TickType_t waitTicks);
    void releaseSpeakerAudio(uint8_t *item);
    void flushBuffers();

private:
    void handleWsEvent(WStype_t type, uint8_t *payload, size_t length);
    void drainMicBufferToServer();

    WebSocketsClient ws_;
    RingbufHandle_t rxRingBuf_ = nullptr;
    RingbufHandle_t txRingBuf_ = nullptr;

    volatile AssistantState currentState_ = AssistantState::IDLE;
    volatile bool ttsFinished_ = false;
};