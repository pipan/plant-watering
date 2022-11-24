#include "WatterQueue.h"

unsigned char WatterQueue::size = 0;
char WatterQueue::queue[WATTER_QUEUE_SIZE];

void WatterQueue::push(char valveIndex) {
    if (!WatterQueue::contains(valveIndex)) {
        WatterQueue::queue[WatterQueue::size++] = valveIndex;
    }
}

char WatterQueue::pop() {
    if (WatterQueue::size == 0) {
        return -1;
    }
    char index = WatterQueue::queue[0];
    WatterQueue::size--;
    if (WatterQueue::size == 0) {
        return index;
    }

    for (unsigned char i = 0; i < WatterQueue::size; i++) {
        WatterQueue::queue[i] = WatterQueue::queue[i + 1];
    }
    return index;
}

bool WatterQueue::contains(char valveIndex) {
    for (unsigned char i = 0; i < WatterQueue::size; i++) {
        if (WatterQueue::queue[i] == valveIndex) {
            return true;
        }
    }
    return false;
}