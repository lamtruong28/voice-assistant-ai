// src/audio_hal.cpp
#include "audio_hal.h"
#include "app_config.h"
#include <cmath>

void AudioHAL::begin()
{
    initMic();
    initSpeaker();
}

void AudioHAL::initMic()
{
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = Config::MIC_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0};

    i2s_pin_config_t pins = {
        .bck_io_num = Config::MIC_SCK_PIN,
        .ws_io_num = Config::MIC_WS_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = Config::MIC_SD_PIN};

    i2s_driver_install(I2S_NUM_0, &config, 0, nullptr);
    i2s_set_pin(I2S_NUM_0, &pins);
    i2s_zero_dma_buffer(I2S_NUM_0);
}

void AudioHAL::initSpeaker()
{
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = Config::SPK_SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 512,
        .use_apll = false,
        .tx_desc_auto_clear = true,
        .fixed_mclk = 0};

    i2s_pin_config_t pins = {
        .bck_io_num = Config::SPK_BCLK_PIN,
        .ws_io_num = Config::SPK_LRC_PIN,
        .data_out_num = Config::SPK_DIN_PIN,
        .data_in_num = I2S_PIN_NO_CHANGE};

    i2s_driver_install(I2S_NUM_1, &config, 0, nullptr);
    i2s_set_pin(I2S_NUM_1, &pins);
    i2s_zero_dma_buffer(I2S_NUM_1);
}

size_t AudioHAL::readProcessedMic(int16_t *outPcmBuffer, size_t maxSamples)
{
    static int32_t rawBuffer[Config::MIC_CHUNK_SAMPLES];
    size_t samplesToRead = min(maxSamples, Config::MIC_CHUNK_SAMPLES);
    size_t bytesRead = 0;

    esp_err_t err = i2s_read(
        I2S_NUM_0,
        rawBuffer,
        samplesToRead * sizeof(int32_t),
        &bytesRead,
        pdMS_TO_TICKS(20));

    if (err != ESP_OK || bytesRead == 0)
    {
        return 0;
    }

    size_t sampleCount = bytesRead / sizeof(int32_t);
    for (size_t i = 0; i < sampleCount; i++)
    {
        // Align 24-bit INMP441 sample to 16-bit range
        int32_t s = rawBuffer[i] >> 14;

        // Exponential moving average DC-offset removal
        dcOffset_ = (dcOffset_ * 999 + s) / 1000;
        s -= dcOffset_;

        // Apply software gain and anti-clipping saturation
        s *= Config::MIC_GAIN_FACTOR;
        if (s > 32767)
            s = 32767;
        if (s < -32768)
            s = -32768;

        outPcmBuffer[i] = static_cast<int16_t>(s);
    }

    return sampleCount;
}

void AudioHAL::writeSpeaker(const uint8_t *data, size_t length)
{
    size_t written = 0;
    i2s_write(I2S_NUM_1, data, length, &written, portMAX_DELAY);
}

void AudioHAL::muteSpeaker()
{
    i2s_zero_dma_buffer(I2S_NUM_1);
}

void AudioHAL::playStartupTone()
{
    const float freq = 440.0f;
    int16_t buffer[256];

    // Play a short 0.3s confirmation chime
    size_t totalSamples = Config::SPK_SAMPLE_RATE * 0.3;
    for (size_t block = 0; block < totalSamples; block += 256)
    {
        for (size_t i = 0; i < 256; i++)
        {
            size_t sample = block + i;
            buffer[i] = static_cast<int16_t>(
                6000.0f * sinf(2.0f * PI * freq * sample / Config::SPK_SAMPLE_RATE));
        }
        writeSpeaker(reinterpret_cast<uint8_t *>(buffer), sizeof(buffer));
    }
    muteSpeaker();
}