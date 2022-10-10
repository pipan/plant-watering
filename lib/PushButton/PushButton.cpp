#include "PushButton.h"
#include "../Time/Time.h"
#include <Arduino.h>

#define PUSH_BUTTON_EVENT_HOLD 2
#define PUSH_BUTTON_EVENT_CLICK 1

PushButton::PushButton(uint8_t pin) : PushButton(pin, HIGH) {}

PushButton::PushButton(uint8_t pin, int pushState) {
    this->pin = pin;
    this->valueAt = 0;
    this->cycleEvent = 0;
    this->pushState = pushState;
}

void PushButton::begin() {
    pinMode(this->pin, INPUT);
    this->value = digitalRead(this->pin);
}

void PushButton::read() {
    int value = digitalRead(this->pin);
    if (value == this->value) {
        return checkHold();
    }
    if (value == this->pushState) {
        triggerPush();
    } else {
        triggerClick();
        triggerRelease();
        this->cycleEvent = 0;
    }
    this->value = value;
    this->valueAt = millis();
}

void PushButton::onClick(void (*fn)()) {
    this->clickFn = fn;
}

void PushButton::onPush(void (*fn)()) {
    this->pushFn = fn;
}

void PushButton::onRelease(void (*fn)()) {
    this->releaseFn = fn;
}

void PushButton::onHold(void (*fn)(), unsigned int treshold) {
    this->holdFn = fn;
    this->holdTreshold = treshold;
}

void PushButton::onHold(void (*fn)()) {
    onHold(fn, 1500);
}

void PushButton::triggerPush() {
    if (this->pushFn == NULL) {
        return;
    }
    this->pushFn();
}

void PushButton::triggerRelease() {
    if (this->releaseFn == NULL) {
        return;
    }
    this->releaseFn();
}

void PushButton::checkHold() {
    if (this->holdFn == NULL) {
        return;
    }
    if (this->value == !this->pushState) {
        return;
    }
    if (this->cycleEvent > 0) {
        return;
    }
    long diff = Time::msDiff(this->valueAt, millis());
    if (diff > this->holdTreshold) {
        this->cycleEvent = PUSH_BUTTON_EVENT_HOLD;
        this->holdFn();
    }
}

void PushButton::triggerClick() {
    if (this->cycleEvent > 0) {
        return;
    }
    if (this->clickFn == NULL) {
        return;
    }
    this->cycleEvent = PUSH_BUTTON_EVENT_CLICK;
    this->clickFn();
}