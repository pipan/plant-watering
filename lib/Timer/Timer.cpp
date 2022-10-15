#include "Timer.h"
#include <Arduino.h>

Timer::Timer() { };

bool Timer::beginHz(unsigned int seconds) {
    unsigned long maxOcr = 65536;
    unsigned long ocrValue = (16000000 / 1024) * seconds - 1;
    if (ocrValue >= maxOcr) {
        return false;
    }
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = ocrValue;
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    TIFR1 = OCF1A;
    TIFR1 = OCF1B;
    TIMSK1 |= (1 << OCIE1A);    
    interrupts();
    return true;
}