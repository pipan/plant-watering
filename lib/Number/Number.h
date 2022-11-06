#ifndef Number_h
#define Number_h

#define constrainCycle(x, low, high) ((x < low) ? (high - low + x + 1) : ((x > high) ? (low + x - high - 1) : x));

class Number {
    public:
        static char decimalLength(int number);
};

#endif