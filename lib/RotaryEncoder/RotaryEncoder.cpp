#include <Arduino.h>
#include "RotaryEncoder.h"

uint8_t RotaryEncoder::PULL_UP_CW_STATES[] = {0b11, 0b01, 0b00, 0b10};

RotaryEncoder::RotaryEncoder(uint8_t clkA, uint8_t dtB) : RotaryEncoder::RotaryEncoder(clkA, dtB, RotaryEncoder::PULL_UP_CW_STATES) {};

RotaryEncoder::RotaryEncoder(uint8_t clkA, uint8_t dtB, uint8_t cwStates[]) {
  this->clkPin = clkA;
  this->dtPin = dtB;
  this->stackItemsSize = 0;
  this->cwStates = cwStates;
  this->state = 0;
};

void RotaryEncoder::begin() {
  pinMode(this->clkPin, INPUT);
  pinMode(this->dtPin, INPUT);
  this->changeState(digitalRead(this->clkPin), digitalRead(this->dtPin));
}

void RotaryEncoder::queue() {
  if (this->changeFn == NULL) {
    return;
  }
  if (this->stackItemsSize >= 16) {
    return;
  }

  this->changeState(digitalRead(this->clkPin), digitalRead(this->dtPin));
  int8_t rotation = this->getRotation();
  if (rotation == -1) {
    return;
  }
  if (rotation) {
    this->stack[this->stackItemsSize / 8] |= (1 << (this->stackItemsSize % 8));
  } else {
    this->stack[this->stackItemsSize / 8] &= ~(1 << (this->stackItemsSize % 8));
  }
  
  this->stackItemsSize++;
  this->state = 0;
};

void RotaryEncoder::unqueue() {
  if (this->changeFn == NULL) {
    return;
  }
  
  noInterrupts();
  for (uint8_t i = 0; i < this->stackItemsSize; i++) {
    this->changeFn((this->stack[i / 8] >> (i % 8)) & 1);
  }
  this->stackItemsSize = 0;
  interrupts();
};

bool RotaryEncoder::unqueueOne() {
  if (this->changeFn == NULL) {
    return false;
  }
  if (this->stackItemsSize == 0) {
    return false;
  }
  
  noInterrupts();
  this->stackItemsSize--;
  bool value = (this->stack[this->stackItemsSize / 8] >> (this->stackItemsSize % 8)) & 1;
  
  interrupts();
  this->changeFn(value);
  return true;
};

void RotaryEncoder::onChange(void (*fn)(bool)) {
  this->changeFn = fn;
};

int8_t RotaryEncoder::getRotation() {
  if (state > 3) {
    return 1;
  }
  if (state < -3) {
    return 0;
  }
  return -1;
};

void RotaryEncoder::changeState(uint8_t clkValue, uint8_t dtValue) {
  uint8_t state = clkValue * 2 + dtValue;
  uint8_t newStateIndex = this->getStateIndex(state);

  int8_t diff = newStateIndex - (this->state % 4);
  if (diff == 3) {
    diff = -1;
  } else if (diff == -3) {
    diff = 1;
  }
  this->state += diff;
};

uint8_t RotaryEncoder::getStateIndex(uint8_t state) {
  for (uint8_t i = 0; i < 4; i++) {
    if (state == this->cwStates[i]) {
      return i;
    }
  }
  return 0;
}