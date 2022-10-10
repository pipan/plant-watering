#ifndef PushButton_h
#define PushButton_h

#include <Arduino.h>

class PushButton {
    public:
        PushButton(uint8_t pin);
        PushButton(uint8_t pin, int pushState);
        void read();
        void begin();
        void onClick(void (*fn)());
        void onHold(void (*fn)());
        void onHold(void (*fn)(), unsigned int treshold);
        void onPush(void (*fn)());
        void onRelease(void (*fn)());
    private:
        uint8_t pin;
        int pushState;
        int value;
        unsigned long valueAt;
        char cycleEvent;
        void (*clickFn)();
        void (*holdFn)();
        void (*pushFn)();
        void (*releaseFn)();
        unsigned int holdTreshold;
        void triggerPush();
        void triggerRelease();
        void triggerClick();
        void checkHold();
};

#endif