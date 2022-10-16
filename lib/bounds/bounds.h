#ifndef bounds_h
#define bounds_h

#include <Arduino.h>

void copyBounds(const uint8_t *source, uint8_t *destination);
void setRowBounds(uint8_t *bounds, uint8_t total, uint8_t position);
void setColumnBounds(uint8_t *bounds, uint8_t total, uint8_t position);
void expandBounds(uint8_t bounds[], uint8_t size);
void expandBounds(uint8_t bounds[], uint8_t vertical, uint8_t horizontal);
void shrinkBounds(uint8_t bounds[], uint8_t size);
void shrinkBounds(uint8_t bounds[], uint8_t vertical, uint8_t horizontal);
void shrinkBoundsRight(uint8_t *bounds, int8_t value);
void setCenterBounds(uint8_t bounds[], uint8_t width, uint8_t height);

#endif