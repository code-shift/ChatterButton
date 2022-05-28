/*!
 * The MIT License (MIT)
 * Copyright (c) 2022 0xdeb <0xdeb@mail.ru>
 */


#include "ChatterButton.h"

#if defined(ARDUINO)
#include <arduino.h>
#else
#include <chrono>
#endif

namespace {   
    unsigned long milliseconds() {
        #if defined(ARDUINO)
        return millis();
        #else
        auto timePoint = std::chrono::steady_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint.time_since_epoch()).count();
        return millis;
        #endif
    }
}

BaseChatterButton::BaseChatterButton(CButtonWire connect) : previousStateDuration{0} {
    closeOnGND = (CButtonWire::GND == connect);
    set(false);
}

BaseChatterButton::~BaseChatterButton() {}

void BaseChatterButton::set(bool state) {
    if (state != this->state) {
        previousStateDuration = getStateDuration();
        lastChangeTime = milliseconds();
    }
    this->state = state;
    wireChangeTime = 0;
    wireAverage = 0;
    hasChange = false;
}

bool BaseChatterButton::didChange() {
    return hasChange;    
}

unsigned long BaseChatterButton::getStateDuration() const {
    return milliseconds() - lastChangeTime;
}

unsigned long BaseChatterButton::getPreviousStateDuration() const {
    return previousStateDuration;
}

//=====================================================================

ChatterButton::ChatterButton(CButtonWire connect) : BaseChatterButton(connect) {}

ChatterButton::operator bool() {
    hasChange = false;
    return state;
}

void ChatterButton::update(bool highLevel) {
    unsigned long time = milliseconds();
    bool lastHighLevel = false;
    if ((state && !closeOnGND) || (!state && closeOnGND)) {
        lastHighLevel = true;
    }

    if (wireChangeTime == 0 && highLevel != lastHighLevel) {
        wireChangeTime = time;
    }
    else if (wireChangeTime != 0) {
        wireAverage += highLevel ? 1 : -1;
        if (time - wireChangeTime > kChatterTimeMs) {
            bool newState = (wireAverage > 0 && !closeOnGND) || (wireAverage < 0 && closeOnGND);
            if (wireAverage == 0 || newState == state) {
                set(state);
            } else {
                set(newState);
                hasChange = true;
                if (handlerFn) handlerFn(newState);
            }
        }
    }
}

void ChatterButton::setHandler(ButtonStateHandler handler) {
    handlerFn = handler;
}

//=====================================================================

ChatterINTButton::ChatterINTButton(CButtonWire connect) : BaseChatterButton(connect), isReady{true}, currentHighLevel{false} 
{}

void ChatterINTButton::updateIfNeeded() {
    if (wireChangeTime == 0) return;
    
    unsigned long time = milliseconds();
    if (time - wireChangeTime > kChatterTimeMs) {
        isReady = true;
        bool newState = currentHighLevel == !closeOnGND;
        if (newState != state) {
            set(newState);
            hasChange = true;
        } else {
            set(state);
        }
    }
}

ChatterINTButton::operator bool() {
    updateIfNeeded();
    hasChange = false;
    return state;
}

bool ChatterINTButton::didChange() {
    updateIfNeeded();
    return BaseChatterButton::didChange();
}

void ChatterINTButton::update(bool highLevel) {
    unsigned long time = milliseconds();
    bool stateHighLevel = false;
    if ((state && !closeOnGND) || (!state && closeOnGND)) {
        stateHighLevel = true;
    }
    
    if (highLevel != stateHighLevel) {
        wireChangeTime = time;
        isReady = false;
    }
    currentHighLevel = highLevel;
}
