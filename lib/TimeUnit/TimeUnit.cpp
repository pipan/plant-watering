#include "TimeUnit.h"
#include <Number.h>
#include <Text.h>

int TimeUnit::increment(int hours) {
    if (hours < 24) {
        return hours + 1;
    }
    if (hours < 72) {
        return hours + 3;
    }
    if (hours < 192) {
        return hours + 8;
    }
    return hours + 24;
}

int TimeUnit::decrement(int hours) {
    if (hours <= 0) {
        return 0;
    }
    if (hours <= 24) {
        return hours - 1;
    }
    if (hours <= 72) {
        return hours - 3;
    }
    if (hours <= 192) {
        return hours - 8;
    }
    return hours - 24;
}