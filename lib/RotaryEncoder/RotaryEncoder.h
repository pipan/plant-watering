#ifndef RotaryEncoder_h
#define RotaryEncoder_h

class RotaryEncoder
{
  public:
    RotaryEncoder(uint8_t clkA, uint8_t dtB);
    RotaryEncoder(uint8_t clkA, uint8_t dtB, uint8_t cwStates[]);
    void begin();
    void queue();
    void unqueue();
    bool unqueueOne();
    void onChange(void (*fn)(bool));
  private:
    static uint8_t PULL_UP_CW_STATES[];
    void (*changeFn)(bool direction);
    uint8_t *cwStates;
    int8_t state;
    uint8_t clkPin;
    uint8_t dtPin;
    uint8_t stack[2];
    uint8_t stackItemsSize;
    int8_t getRotation();
    void changeState(uint8_t clkValue, uint8_t dtValue);
    uint8_t getStateIndex(uint8_t state);
};

#endif