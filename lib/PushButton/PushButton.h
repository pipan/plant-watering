#ifndef PushButton_h
#define PushButton_h

#include <Arduino.h>

class PushButton {
    public:
        PushButton(uint8_t pin);
        PushButton(uint8_t pin, uint8_t pushState);
        static const uint8_t PUSH_BUTTON_EVENT_CLICK;
        static const uint8_t PUSH_BUTTON_EVENT_HOLD;
        void read();
        void begin();
        void onClick(void (*fn)());
        void onHold(void (*fn)());
        void onHold(void (*fn)(), uint16_t treshold);
        void onPush(void (*fn)());
        void onRelease(void (*fn)());
    private:
        uint8_t pin;
        uint8_t pushState;
        uint8_t value;
        uint64_t valueAt;
        uint8_t cycleEvent;
        uint16_t holdTreshold;
        void (*clickFn)();
        void (*holdFn)();
        void (*pushFn)();
        void (*releaseFn)();
        void triggerPush();
        void triggerRelease();
        void triggerClick();
        void checkHold();
};

#endif