#pragma once

/**
 * @brief Represents the operational states of the Voice Assistant.
 */
enum class AssistantState
{
    IDLE,       // Connected and ready for user input (LED OFF)
    LISTENING,  // Capturing audio from INMP441 and streaming to server (LED SOLID ON)
    PROCESSING, // Waiting for STT/LLM/TTS inference from server (LED FAST BLINK)
    SPEAKING    // Playing back TTS PCM stream via MAX98357A (LED SLOW BLINK)
};