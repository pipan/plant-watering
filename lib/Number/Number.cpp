#include "Number.h"
#include <math.h>

char Number::decimalLength(int number) {
    char length = 0;
    while (number > 0) {
        number = number / 10;
        length++;
    }
    return length < 1 ? 1 : length;
}