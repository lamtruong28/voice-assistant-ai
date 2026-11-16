// src/audio_hal.h
#pragma once

#include <Arduino.h>
#include <driver/i2s.h>

/**
 * @brief Hardware Abstraction Layer for I2S Microphone (INMP441) and Amplifier (MAX98357A).
 */
class AudioHAL
{
public:
    void begin();

    /**
     * @brief Read raw 32-bit I2S frames from INMP441, apply DC offset removal and gain,
     *        and output 16-bit PCM samples.
     * @param outPcmBuffer Destination buffer for 16-bit PCM samples.
     * @param maxSamples Maximum number of samples to read.
     * @return Number of 16-bit samples actually processed.
     */
    size_t readProcessedMic(int16_t *outPcmBuffer, size_t maxSamples);

    /**
     * @brief Write raw 16-bit mono PCM bytes to the MAX98357A speaker amplifier.
     */
    void writeSpeaker(const uint8_t *data, size_t length);

    /**
     * @brief Clear speaker DMA buffers immediately to prevent residual noise or hum.
     */
    void muteSpeaker();

    /**
     * @brief Play a brief 440Hz diagnostic sine tone on startup.
     */
    void playStartupTone();

private:
    void initMic();
    void initSpeaker();
    int32_t dcOffset_ = 0;
};