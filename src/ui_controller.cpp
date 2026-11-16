// src/ui_controller.cpp
#include "ui_controller.h"
#include "app_config.h"

void UIController::begin()
{
    pinMode(Config::LED_PIN, OUTPUT);
    // Keep external pull-up configuration matching your physical circuit
    pinMode(Config::BUTTON_REC_PIN, INPUT);
    digitalWrite(Config::LED_PIN, LOW);
}

bool UIController::isButtonPressed()
{
    bool triggered = false;
    bool reading = digitalRead(Config::BUTTON_REC_PIN);

    if (reading != lastReading_)
    {
        lastDebounceTime_ = millis();
    }

    if ((millis() - lastDebounceTime_) > Config::DEBOUNCE_DELAY_MS)
    {
        if (reading != buttonState_)
        {
            buttonState_ = reading;
            if (buttonState_ == LOW)
            {
                triggered = true;
            }
        }
    }

    lastReading_ = reading;
    return triggered;
}

void UIController::updateLED(AssistantState state)
{
    unsigned long now = millis();

    switch (state)
    {
    case AssistantState::IDLE:
        digitalWrite(Config::LED_PIN, LOW);
        break;

    case AssistantState::LISTENING:
        digitalWrite(Config::LED_PIN, HIGH);
        break;

    case AssistantState::PROCESSING:
        if (now - lastLedToggle_ >= 100)
        { // Fast blink (5Hz)
            lastLedToggle_ = now;
            ledOutput_ = !ledOutput_;
            digitalWrite(Config::LED_PIN, ledOutput_);
        }
        break;

    case AssistantState::SPEAKING:
        if (now - lastLedToggle_ >= 400)
        { // Slow blink (1.25Hz)
            lastLedToggle_ = now;
            ledOutput_ = !ledOutput_;
            digitalWrite(Config::LED_PIN, ledOutput_);
        }
        break;
    }
}