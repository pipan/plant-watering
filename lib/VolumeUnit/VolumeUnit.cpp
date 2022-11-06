#include "VolumeUnit.h"
#include <Number.h>
#include <Text.h>

int VolumeUnit::increment(int volume) {
    if (volume < 1000) {
        return volume + 50;
    }
    if (volume < 3000) {
        return volume + 100;
    }
    return volume + 250;
}

int VolumeUnit::decrement(int volume) {
    if (volume <= 0) {
        return 0;
    }
    if (volume <= 1000) {
        return volume - 50;
    }
    if (volume <= 3000) {
        return volume - 100;
    }
    return volume - 250;
}