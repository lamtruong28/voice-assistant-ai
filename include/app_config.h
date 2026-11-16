#pragma once

#include <Arduino.h>

// ============================================================================
// Network & Server Configuration
// ============================================================================
namespace Config
{
    constexpr const char *WIFI_SSID = "Truong";
    constexpr const char *WIFI_PASS = "@T280299";
    constexpr const char *WS_HOST = "192.168.1.7";
    constexpr uint16_t WS_PORT = 5000;
    constexpr const char *WS_PATH = "/";
    constexpr uint32_t WS_RECONNECT_MS = 3000;

    // ========================================================================
    // Hardware Pinout (ESP32-S3 SuperMini)
    // ========================================================================
    // INMP441 Microphone (I2S_NUM_0 - RX)
    constexpr int MIC_SD_PIN = 9;
    constexpr int MIC_WS_PIN = 10;
    constexpr int MIC_SCK_PIN = 11;

    // MAX98357A Amplifier (I2S_NUM_1 - TX)
    constexpr int SPK_DIN_PIN = 5;
    constexpr int SPK_BCLK_PIN = 6;
    constexpr int SPK_LRC_PIN = 7;

    // User Interface Peripherals
    constexpr int LED_PIN = 3;
    constexpr int BUTTON_REC_PIN = 4;

    // ========================================================================
    // Audio & DSP Parameters
    // ========================================================================
    constexpr uint32_t MIC_SAMPLE_RATE = 16000;
    constexpr uint32_t SPK_SAMPLE_RATE = 22050;
    constexpr size_t MIC_CHUNK_SAMPLES = 320; // 320 samples = 640 bytes (20ms audio)
    constexpr size_t SPK_CHUNK_BYTES = 1024;  // Matches backend stream chunk size
    constexpr int32_t MIC_GAIN_FACTOR = 4;    // Software gain multiplier

    // ========================================================================
    // FreeRTOS & RingBuffer Tuning (512KB Internal SRAM Safe)
    // ========================================================================
    // constexpr size_t RX_RINGBUF_BYTES = 16 * 1024; // 16KB Mic TX buffer
    // constexpr size_t TX_RINGBUF_BYTES = 48 * 1024; // 48KB Speaker RX buffer (~1.1s)
    // constexpr uint32_t DEBOUNCE_DELAY_MS = 50;

    // ========================================================================
    // FreeRTOS & RingBuffer Tuning
    // ========================================================================
    constexpr size_t RX_RINGBUF_BYTES = 16 * 1024; // 16KB Mic TX buffer (Internal SRAM)
    constexpr size_t TX_RINGBUF_BYTES = 96 * 1024; // 96KB Speaker RX buffer (~2.2s in PSRAM)
    constexpr uint32_t DEBOUNCE_DELAY_MS = 50;
}