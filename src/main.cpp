// #include <Arduino.h>
// #include <Wire.h>
// #include <driver/i2s.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64

// #define LED_PIN 48

// #define OLED_SDA 12
// #define OLED_SCL 13

// #define I2S_SD 9
// #define I2S_WS 10
// #define I2S_SCK 11

// Adafruit_SSD1306 display(
//     SCREEN_WIDTH,
//     SCREEN_HEIGHT,
//     &Wire,
//     -1);

// unsigned long lastBlink = 0;
// bool ledState = false;

// float smoothLevel = 0;

// // =========================
// // Faces
// // =========================

// void drawSleepFace()
// {
//   display.drawLine(35, 22, 50, 22, WHITE);
//   display.drawLine(78, 22, 93, 22, WHITE);

//   display.drawLine(54, 44, 74, 44, WHITE);
// }

// void drawHappyFace()
// {
//   display.fillCircle(42, 22, 4, WHITE);
//   display.fillCircle(86, 22, 4, WHITE);

//   display.drawLine(54, 44, 74, 44, WHITE);
//   display.drawLine(54, 44, 58, 48, WHITE);
//   display.drawLine(74, 44, 70, 48, WHITE);
// }

// void drawSurprisedFace()
// {
//   display.drawCircle(42, 22, 5, WHITE);
//   display.drawCircle(86, 22, 5, WHITE);

//   display.drawCircle(64, 46, 5, WHITE);
// }

// void drawAngryFace()
// {
//   display.drawLine(35, 18, 50, 28, WHITE);
//   display.drawLine(78, 28, 93, 18, WHITE);

//   display.drawLine(54, 45, 74, 45, WHITE);
// }

// // =========================
// // Setup
// // =========================

// void setup()
// {
//   Serial.begin(115200);

//   pinMode(LED_PIN, OUTPUT);

//   Wire.begin(OLED_SDA, OLED_SCL);

//   if (!display.begin(
//           SSD1306_SWITCHCAPVCC,
//           0x3C))
//   {
//     while (true)
//       ;
//   }

//   display.clearDisplay();

//   display.setTextSize(1);
//   display.setTextColor(WHITE);
//   display.setCursor(18, 28);
//   display.println("MOCHI BOOT");

//   display.display();

//   delay(1000);

//   i2s_config_t i2s_config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//       .sample_rate = 16000,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 128,
//       .use_apll = false,
//       .tx_desc_auto_clear = false,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pin_config = {
//       .bck_io_num = I2S_SCK,
//       .ws_io_num = I2S_WS,
//       .data_out_num = I2S_PIN_NO_CHANGE,
//       .data_in_num = I2S_SD};

//   i2s_driver_install(
//       I2S_NUM_0,
//       &i2s_config,
//       0,
//       NULL);

//   i2s_set_pin(
//       I2S_NUM_0,
//       &pin_config);

//   Serial.println("INMP441 READY");
// }

// // =========================
// // Loop
// // =========================

// void loop()
// {
//   // Blink LED

//   if (millis() - lastBlink > 500)
//   {
//     lastBlink = millis();

//     ledState = !ledState;

//     digitalWrite(
//         LED_PIN,
//         ledState);
//   }

//   // =====================
//   // Read microphone
//   // =====================
//   int32_t samples[256];

//   size_t bytesRead = 0;

//   i2s_read(
//       I2S_NUM_0,
//       samples,
//       sizeof(samples),
//       &bytesRead,
//       portMAX_DELAY);

//   long peak = 0;

//   int count = bytesRead / sizeof(int32_t);

//   for (int i = 0; i < count; i++)
//   {
//     long value = abs(samples[i] >> 14);

//     if (value > peak)
//       peak = value;
//   }

//   Serial.println(peak);

//   delay(50);

//   // =====================
//   // Draw OLED
//   // =====================

//   display.clearDisplay();

//   display.setTextSize(1);
//   display.setTextColor(WHITE);

//   display.setCursor(0, 0);
//   display.println("MOCHI LISTENING...");

//   if (smoothLevel < 5)
//   {
//     drawSleepFace();

//     display.setCursor(0, 40);
//     display.print("SLEEP");
//   }
//   else if (smoothLevel < 20)
//   {
//     drawHappyFace();

//     display.setCursor(0, 40);
//     display.print("HAPPY");
//   }
//   else if (smoothLevel < 50)
//   {
//     drawSurprisedFace();

//     display.setCursor(0, 40);
//     display.print("LISTEN");
//   }
//   else
//   {
//     drawAngryFace();

//     display.setCursor(0, 40);
//     display.print("LOUD");
//   }

//   display.drawRect(
//       4,
//       54,
//       120,
//       8,
//       WHITE);

//   // display.fillRect(
//   //     5,
//   //     55,
//   //     bar,
//   //     6,
//   //     WHITE);

//   display.display();
// }

// ==========================================================================
// TEST MIC
// ==========================================================================
// #include <Arduino.h>
// #include <driver/i2s.h>

// #define I2S_SD 9
// #define I2S_WS 10
// #define I2S_SCK 11

// #define SAMPLE_RATE 16000
// #define BUFFER_SIZE 256

// void setup()
// {
//   Serial.begin(115200);

//   i2s_config_t config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//       .sample_rate = SAMPLE_RATE,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 64,
//       .use_apll = false,
//       .tx_desc_auto_clear = false,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pins = {
//       .bck_io_num = I2S_SCK,
//       .ws_io_num = I2S_WS,
//       .data_out_num = I2S_PIN_NO_CHANGE,
//       .data_in_num = I2S_SD};

//   i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
//   i2s_set_pin(I2S_NUM_0, &pins);

//   Serial.println("INMP441 Ready");
// }

// void loop()
// {
//   static int32_t buffer[BUFFER_SIZE];

//   size_t bytesRead = 0;

//   i2s_read(
//       I2S_NUM_0,
//       buffer,
//       sizeof(buffer),
//       &bytesRead,
//       portMAX_DELAY);

//   int samples = bytesRead / sizeof(int32_t);

//   //
//   // INMP441 24-bit -> align
//   //
//   int64_t sum = 0;

//   for (int i = 0; i < samples; i++)
//   {
//     sum += (buffer[i] >> 14);
//   }

//   int32_t dc = sum / samples;

//   long peak = 0;
//   long rms = 0;

//   for (int i = 0; i < samples; i++)
//   {
//     int32_t s = (buffer[i] >> 14);

//     // remove DC offset
//     s -= dc;

//     long v = abs(s);

//     if (v > peak)
//       peak = v;

//     rms += ((long)s * (long)s);
//   }

//   rms = sqrt((float)rms / samples);

//   Serial.printf(
//       "DC=%ld RMS=%ld PEAK=%ld\n",
//       dc,
//       rms,
//       peak);

//   delay(50);
// }

// ====================================================
// MIC + LOA OK
// ====================================================
// #include <Arduino.h>
// #include <driver/i2s.h>

// // =========================
// // INMP441
// // =========================
// #define I2S_SD 9
// #define I2S_WS 10
// #define I2S_SCK 11

// // =========================
// // MAX98357A
// // =========================
// #define DIN 5
// #define BCLK 6
// #define LRC 7

// #define SAMPLE_RATE 16000
// #define BUFFER_SIZE 256

// // 0.02 = 2% volume
// // thử 0.01 nếu còn hú
// const float VOLUME = 0.15f;

// static int32_t dc = 0;

// void setupMic()
// {
//   i2s_config_t config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//       .sample_rate = SAMPLE_RATE,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 64,
//       .use_apll = false,
//       .tx_desc_auto_clear = false,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pins = {
//       .bck_io_num = I2S_SCK,
//       .ws_io_num = I2S_WS,
//       .data_out_num = I2S_PIN_NO_CHANGE,
//       .data_in_num = I2S_SD};

//   i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
//   i2s_set_pin(I2S_NUM_0, &pins);
// }

// void setupSpeaker()
// {
//   i2s_config_t config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
//       .sample_rate = SAMPLE_RATE,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
//       .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = 0,
//       .dma_buf_count = 8,
//       .dma_buf_len = 64,
//       .use_apll = false,
//       .tx_desc_auto_clear = true,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pins = {
//       .bck_io_num = BCLK,
//       .ws_io_num = LRC,
//       .data_out_num = DIN,
//       .data_in_num = I2S_PIN_NO_CHANGE};

//   i2s_driver_install(I2S_NUM_1, &config, 0, NULL);
//   i2s_set_pin(I2S_NUM_1, &pins);
// }

// void setup()
// {
//   Serial.begin(115200);

//   setupMic();
//   setupSpeaker();

//   Serial.println("Mic -> Speaker Started");
// }

// void loop()
// {
//   static int32_t micBuffer[BUFFER_SIZE];
//   static int16_t spkBuffer[BUFFER_SIZE * 2];

//   size_t bytesRead = 0;

//   i2s_read(
//       I2S_NUM_0,
//       micBuffer,
//       sizeof(micBuffer),
//       &bytesRead,
//       portMAX_DELAY);

//   int samples = bytesRead / sizeof(int32_t);

//   for (int i = 0; i < samples; i++)
//   {
//     // Align INMP441 data
//     int32_t s = micBuffer[i] >> 14;

//     // DC removal
//     dc = (dc * 999 + s) / 1000;
//     s -= dc;

//     // Software volume
//     s = (int32_t)(s * VOLUME);

//     // Noise gate
//     if (abs(s) < 10)
//       s = 0;

//     // Anti clipping
//     if (s > 32767)
//       s = 32767;

//     if (s < -32768)
//       s = -32768;

//     int16_t out = (int16_t)s;

//     // Stereo output
//     spkBuffer[i * 2] = out;
//     spkBuffer[i * 2 + 1] = out;
//   }

//   size_t bytesWritten;

//   i2s_write(
//       I2S_NUM_1,
//       spkBuffer,
//       samples * 2 * sizeof(int16_t),
//       &bytesWritten,
//       portMAX_DELAY);
// }

// #include <Arduino.h>
// #include <WiFi.h>
// #include <WebSocketsClient.h>
// #include <driver/i2s.h>

// #define WIFI_SSID "Truong"
// #define WIFI_PASS "@T280299"

// // Mini PC IP
// #define WS_HOST "192.168.1.7"
// #define WS_PORT 5000

// // INMP441 & MIC
// #define I2S_SD 9
// #define I2S_WS 10
// #define I2S_SCK 11

// // Button and led
// #define LED 3
// #define BUTTON_REC 4

// // Speaker
// #define SPK_DIN 5
// #define SPK_BCLK 6
// #define SPK_LRC 7

// bool buttonState = LOW;
// bool isRecording = false;
// bool lastReading = HIGH;
// bool isPlaying = false;

// unsigned long lastDebounceTime = 0;
// const unsigned long debounceDelay = 50;

// #define SAMPLE_RATE 16000
// #define PCM_SAMPLES 320

// WebSocketsClient ws;

// int32_t micBuffer[PCM_SAMPLES];
// int16_t pcmBuffer[PCM_SAMPLES];

// void onWsEvent(
//     WStype_t type,
//     uint8_t *payload,
//     size_t length)
// {
//   switch (type)
//   {
//   case WStype_CONNECTED:
//     Serial.println("WS Connected");
//     break;

//   case WStype_DISCONNECTED:
//     Serial.println("WS Disconnected");
//     break;

//   case WStype_TEXT:
//   {
//     String msg = (char *)payload;
//     if (msg == "tts:start")
//     {
//       isPlaying = true;
//       Serial.println("TTS START");
//     }
//     else if (msg == "tts:end")
//     {
//       isPlaying = false;
//       Serial.println("TTS END");
//     }

//     break;
//   }

//   case WStype_BIN:
//   {
//     if (!isPlaying)
//     {
//       return;
//     }

//     size_t written;

//     i2s_write(
//         I2S_NUM_1,
//         payload,
//         length,
//         &written,
//         portMAX_DELAY);

//     break;
//   }

//   default:
//     break;
//   }
// }

// void setupMic()
// {
//   i2s_config_t config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
//       .sample_rate = SAMPLE_RATE,
//       .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
//       .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 64,
//       .use_apll = false,
//       .tx_desc_auto_clear = false,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pins = {
//       .bck_io_num = I2S_SCK,
//       .ws_io_num = I2S_WS,
//       .data_out_num = I2S_PIN_NO_CHANGE,
//       .data_in_num = I2S_SD};

//   i2s_driver_install(I2S_NUM_0, &config, 0, NULL);
//   i2s_set_pin(I2S_NUM_0, &pins);
// }

// void setupSpeaker()
// {
//   i2s_config_t config = {
//       .mode = (i2s_mode_t)(I2S_MODE_MASTER |
//                            I2S_MODE_TX),
//       .sample_rate = 22050,
//       .bits_per_sample =
//           I2S_BITS_PER_SAMPLE_16BIT,
//       .channel_format =
//           I2S_CHANNEL_FMT_ONLY_LEFT,
//       .communication_format =
//           I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags =
//           ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 8,
//       .dma_buf_len = 512,
//       .use_apll = false,
//       .tx_desc_auto_clear = true,
//       .fixed_mclk = 0};

//   i2s_pin_config_t pins = {
//       .bck_io_num = SPK_BCLK,
//       .ws_io_num = SPK_LRC,
//       .data_out_num = SPK_DIN,
//       .data_in_num = I2S_PIN_NO_CHANGE};

//   i2s_driver_install(
//       I2S_NUM_1,
//       &config,
//       0,
//       NULL);

//   i2s_set_pin(
//       I2S_NUM_1,
//       &pins);
// }

// void setupWifi()
// {
//   WiFi.mode(WIFI_STA);

//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   Serial.print("Connecting WiFi");

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println();
//   Serial.print("IP: ");
//   Serial.println(WiFi.localIP());
// }

// void setupWebSocket()
// {
//   ws.begin(WS_HOST, WS_PORT, "/");
//   ws.onEvent(onWsEvent);
//   ws.setReconnectInterval(5000);
// }

// void handleButton()
// {
//   bool reading = digitalRead(BUTTON_REC);
//   if (reading != lastReading)
//   {
//     lastDebounceTime = millis();
//   }

//   if (millis() - lastDebounceTime > debounceDelay)
//   {
//     if (reading != buttonState)
//     {
//       buttonState = reading;
//       if (buttonState == LOW)
//       {
//         isRecording = !isRecording;
//         if (isRecording)
//         {
//           Serial.println("START RECORDING");
//           digitalWrite(LED, HIGH);
//           ws.sendTXT("start");
//         }
//         else
//         {
//           Serial.println("STOP RECORDING");

//           digitalWrite(LED, LOW);
//           ws.sendTXT("stop");
//         }
//       }
//     }
//   }

//   lastReading = reading;
// }

// void streamAudio()
// {
//   size_t bytesRead = 0;

//   i2s_read(
//       I2S_NUM_0,
//       micBuffer,
//       sizeof(micBuffer),
//       &bytesRead,
//       portMAX_DELAY);

//   int samples = bytesRead / sizeof(int32_t);

//   static int32_t dc = 0;

//   for (int i = 0; i < samples; i++)
//   {
//     int32_t s = micBuffer[i] >> 14;

//     dc = (dc * 999 + s) / 1000;
//     s -= dc;
//     s *= 4;

//     if (s > 32767)
//       s = 32767;

//     if (s < -32768)
//       s = -32768;

//     pcmBuffer[i] = (int16_t)s;
//   }

//   ws.sendBIN((uint8_t *)pcmBuffer, samples * sizeof(int16_t));
// }
// void testSpeaker()
// {
//   const int SAMPLE_RATE_SPEAKER = 22050;
//   const int FREQ = 440;

//   int16_t buffer[256];

//   for (int block = 0; block < 22050; block += 256)
//   {
//     for (int i = 0; i < 256; i++)
//     {
//       int sample =
//           block + i;

//       buffer[i] =
//           (int16_t)(8000 *
//                     sin(
//                         2.0 *
//                         PI *
//                         FREQ *
//                         sample /
//                         SAMPLE_RATE_SPEAKER));
//     }

//     size_t written;

//     i2s_write(
//         I2S_NUM_1,
//         buffer,
//         sizeof(buffer),
//         &written,
//         portMAX_DELAY);
//   }
// }
// void setup()
// {
//   Serial.begin(115200);
//   pinMode(LED, OUTPUT);
//   pinMode(BUTTON_REC, INPUT);
//   digitalWrite(LED, LOW);
//   setupMic();
//   setupSpeaker();
//   setupWifi();
//   setupWebSocket();

//   Serial.println("Audio Stream Started");
//   testSpeaker();
// }

// void loop()
// {
//   ws.loop();
//   handleButton();
//   if (!isRecording)
//   {
//     delay(5);
//     return;
//   }
//   if (!ws.isConnected())
//   {
//     delay(10);
//     return;
//   }
//   streamAudio();
// }

// #include <Arduino.h>

// #define LED 3
// #define BUTTON 4
// int buttonState;
// void setup()
// {
//   Serial.begin(9600);
//   pinMode(LED, OUTPUT);
//   pinMode(BUTTON, INPUT);
//   digitalWrite(LED, LOW);
// }

// void loop()
// {
//   buttonState = digitalRead(BUTTON);
//   if (buttonState == HIGH)
//   {
//     digitalWrite(LED, HIGH);
//   }
//   else
//   {
//     digitalWrite(LED, LOW);
//   }
//   delay(200);
// }

/**
 * @file main.cpp
 * @brief Enterprise Dual-Core Voice Assistant Entry Point for ESP32-S3 SuperMini.
 */

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "app_config.h"
#include "app_state.h"
#include "audio_hal.h"
#include "ui_controller.h"
#include "ws_transport.h"

AudioHAL audioHAL;
UIController uiCtrl;
WsTransport transport;

/**
 * @brief Core 0 Task: Handles Network I/O, WebSocket loop, Button input, and LED updates.
 */
void networkAndUITask(void *pvParameters)
{
  while (true)
  {
    transport.loop();

    AssistantState state = transport.getState();
    uiCtrl.updateLED(state);

    if (uiCtrl.isButtonPressed())
    {
      if (state == AssistantState::IDLE || state == AssistantState::SPEAKING)
      {
        // Start listening (also acts as Barge-in if assistant is speaking)
        audioHAL.muteSpeaker();
        transport.triggerStartListening();
      }
      else if (state == AssistantState::LISTENING)
      {
        transport.triggerStopListening();
      }
    }

    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

/**
 * @brief Core 1 Task: Real-time deterministic Audio I/O (I2S Mic RX & I2S Speaker TX).
 */
void audioIOTask(void *pvParameters)
{
  int16_t micPcmBuffer[Config::MIC_CHUNK_SAMPLES];

  while (true)
  {
    AssistantState state = transport.getState();

    // 1. Capture & preprocess microphone audio when listening
    if (state == AssistantState::LISTENING)
    {
      size_t samplesRead = audioHAL.readProcessedMic(micPcmBuffer, Config::MIC_CHUNK_SAMPLES);
      if (samplesRead > 0)
      {
        transport.pushMicAudio(
            reinterpret_cast<uint8_t *>(micPcmBuffer),
            samplesRead * sizeof(int16_t));
      }
    }
    // 2. Drain speaker RingBuffer and play via I2S when speaking
    else if (state == AssistantState::SPEAKING)
    {
      size_t itemSize = 0;
      uint8_t *spkChunk = transport.popSpeakerAudio(
          &itemSize,
          Config::SPK_CHUNK_BYTES,
          pdMS_TO_TICKS(10));

      if (spkChunk != nullptr && itemSize > 0)
      {
        audioHAL.writeSpeaker(spkChunk, itemSize);
        transport.releaseSpeakerAudio(spkChunk);
      }
      else if (transport.isTtsFinished())
      {
        audioHAL.muteSpeaker();
        transport.clearTtsFinished();
        transport.setState(AssistantState::IDLE);
        Serial.println("[STATE] -> IDLE");
      }
    }
    // 3. Yield CPU cleanly when idle or waiting for server inference
    else
    {
      vTaskDelay(pdMS_TO_TICKS(5));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Enterprise Voice Assistant Booting ===");

  uiCtrl.begin();
  audioHAL.begin();
  audioHAL.playStartupTone();

  if (!transport.begin())
  {
    while (true)
    {
      delay(1000);
    }
  }

  // Pin deterministic audio loop to Core 1, network/UI loop to Core 0
  xTaskCreatePinnedToCore(networkAndUITask, "NetUITask", 8192, nullptr, 2, nullptr, 0);
  xTaskCreatePinnedToCore(audioIOTask, "AudioTask", 8192, nullptr, 3, nullptr, 1);

  Serial.printf("[SYS] Ready! Free Heap: %lu bytes\n", (unsigned long)ESP.getFreeHeap());
}

void loop()
{
  vTaskDelete(nullptr);
}