#include "bounds.h"
#include <Arduino.h>

void copyBounds(const uint8_t *source, uint8_t *destination) {
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
}

void setRowBounds(uint8_t *bounds, uint8_t total, uint8_t position) {
    bounds[3] = bounds[3] / total;
    bounds[1] += position * bounds[3];
}

void setColumnBounds(uint8_t *bounds, uint8_t total, uint8_t position) {
    bounds[2] = bounds[2] / total;
    bounds[0] += position * bounds[2];
}

void expandBounds(uint8_t bounds[], uint8_t size) {
    expandBounds(bounds, size, size);
}

void expandBounds(uint8_t bounds[], uint8_t vertical, uint8_t horizontal) {
    bounds[0] -= min(bounds[0], horizontal);
    bounds[1] -= min(bounds[1], vertical);
    bounds[2] += 2 * horizontal;
    bounds[3] += 2 * vertical;
}

void shrinkBounds(uint8_t *bounds, uint8_t size) {
    shrinkBounds(bounds, size, size);
}

void shrinkBounds(uint8_t *bounds, uint8_t vertical, uint8_t horizontal) {
    bounds[0] += horizontal;
    bounds[1] += vertical;
    bounds[2] -= min(2 * horizontal, bounds[2]);
    bounds[3] -= min(2 * vertical, bounds[3]);
}

void shrinkBoundsRight(uint8_t *bounds, int8_t value) {
    bounds[2] -= value;
}

void setCenterBounds(uint8_t bounds[], uint8_t width, uint8_t height) {
    uint8_t left = (bounds[2] - width) / 2;
    uint8_t top = (bounds[3] - height) / 2;
    bounds[0] += left;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}