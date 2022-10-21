#include "CursorAnimation.h"
#include <Arduino.h>

CursorAnimation::CursorAnimation() {
    this->animationTime = -1;
}

bool CursorAnimation::tick(int msDiff) {
    if (this->animationTime == -1) {
        return false;
    }
    int nextTime = (this->animationTime + msDiff) % 500;
    bool changed = nextTime / 250 != this->animationTime / 250;
    this->animationTime = nextTime;
    return changed;
}

char CursorAnimation::getState() {
    if (this->animationTime == -1) {
        return -1;
    }
    return this->animationTime / 250;
}

void CursorAnimation::toggle() {
    this->animationTime = this->animationTime == -1 ? 0 : -1;
}