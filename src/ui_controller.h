// src/ui_controller.h
#pragma once

#include <Arduino.h>
#include "app_state.h"

/**
 * @brief Handles physical push-to-talk button debouncing and LED state feedback.
 */
class UIController
{
public:
    void begin();

    /**
     * @brief Poll button state with debounce.
     * @return true on a clean falling-edge button press event.
     */
    bool isButtonPressed();

    /**
     * @brief Update status LED pattern according to the current assistant state.
     */
    void updateLED(AssistantState state);

private:
    bool buttonState_ = HIGH;
    bool lastReading_ = HIGH;
    unsigned long lastDebounceTime_ = 0;
    unsigned long lastLedToggle_ = 0;
    bool ledOutput_ = false;
};