#include "OledComponents.h"
#include <U8g2lib.h>

OledComponents::OledComponents(U8G2 *oled) {
    this->oled = oled;
}

void OledComponents::setFullScreenBounds(uint16_t *bounds) {
    bounds[0] = 0;
    bounds[1] = 0;
    bounds[2] = this->oled->getDisplayWidth();
    bounds[3] = this->oled->getDisplayHeight();
}

void OledComponents::copyBounds(uint16_t *source, uint16_t *destination) {
    for (unsigned char i = 0; i < 4; i++) {
        destination[i] = source[i];
    }
}

void OledComponents::expandBounds(uint16_t *bounds, int8_t size) {
    this->expandBounds(bounds, size, size);
}

void OledComponents::expandBounds(uint16_t *bounds, int8_t vertical, int8_t horizontal) {
    bounds[0] -= min(horizontal, bounds[0]);
    bounds[1] -= min(vertical, bounds[1]);
    bounds[2] += 2 * horizontal;
    bounds[3] += 2 * vertical;
}

void OledComponents::shrinkBounds(uint16_t *bounds, int8_t size) {
    this->shrinkBounds(bounds, size, size);
}

void OledComponents::shrinkBounds(uint16_t *bounds, int8_t vertical, int8_t horizontal) {
    bounds[0] += horizontal;
    bounds[1] += vertical;
    bounds[2] -= min(2 * horizontal, bounds[2]);
    bounds[3] -= min(2 * vertical, bounds[3]);
}

void OledComponents::shrinkBoundsRight(uint16_t *bounds, int8_t value) {
    bounds[2] -= value;
}

void OledComponents::setCenterTextBounds(uint16_t bounds[], char *text) {
    uint16_t width = this->oled->getStrWidth(text);
    uint16_t height = this->oled->getFontAscent();
    this->setCenterBounds(bounds, width, height);
}

void OledComponents::setLeftTextBounds(uint16_t bounds[], char *text) {
    uint16_t width = this->oled->getStrWidth(text);
    uint16_t height = this->oled->getFontAscent();
    uint16_t top = (bounds[3] - height) / 2;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::setRightTextBounds(uint16_t bounds[], char *text) {
    uint16_t width = this->oled->getStrWidth(text);
    uint16_t height = this->oled->getFontAscent();
    uint16_t top = (bounds[3] - height) / 2;
    bounds[0] = bounds[0] + bounds[2] - width;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::setCenterBounds(uint16_t bounds[], uint16_t width, uint16_t height) {
    uint16_t left = (bounds[2] - width) / 2;
    uint16_t top = (bounds[3] - height) / 2;
    bounds[0] += left;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::setRightTextBounds(uint16_t bounds[], char *text, uint8_t start, uint8_t length) {
    char subText[length + 1];
    for (int i = 0; i < length; i++) {
        subText[i] = text[start + i];
    }
    subText[length] = '\0';
    char prefixText[start + 1];
    for (int i = 0; i < start; i++) {
        prefixText[i] = text[i];
    }
    prefixText[start] = '\0';
    uint16_t fullWidth = this->oled->getStrWidth(text);
    uint16_t width = this->oled->getStrWidth(subText);
    uint16_t prefixWidth = this->oled->getStrWidth(prefixText);
    uint16_t height = this->oled->getFontAscent();
    uint16_t top = (bounds[3] - height) / 2;
    bounds[0] = bounds[0] + bounds[2] - fullWidth + prefixWidth;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::drawTextCenter(uint16_t bounds[], char *text) {
    uint16_t textBounds[4];
    this->copyBounds(bounds, textBounds);
    this->setCenterTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawTextLeft(uint16_t bounds[], char *text) {
    uint16_t textBounds[4];
    this->copyBounds(bounds, textBounds);
    this->setLeftTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawTextRight(uint16_t bounds[], char *text) {
    uint16_t textBounds[4];
    this->copyBounds(bounds, textBounds);
    this->setRightTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawFocus(uint16_t bounds[]) {
    uint8_t size = 5;
    this->oled->drawHLine(bounds[0], bounds[1], size);
    this->oled->drawHLine(bounds[0], bounds[1] + bounds[3] - 1, size);
    this->oled->drawHLine(bounds[0] + bounds[2] - 1 - size, bounds[1], size);
    this->oled->drawHLine(bounds[0] + bounds[2] - 1 - size, bounds[1] + bounds[3] - 1, size);

    this->oled->drawVLine(bounds[0], bounds[1], size);
    this->oled->drawVLine(bounds[0], bounds[1] + bounds[3] - size - 1, size);
    this->oled->drawVLine(bounds[0] + bounds[2] - 1, bounds[1], size);
    this->oled->drawVLine(bounds[0] + bounds[2] - 1, bounds[1] + bounds[3] - size - 1, size);
}

void OledComponents::drawScrollBar(uint16_t *bounds, int8_t length, int8_t index) {
    if (length <= 1) {
        return;
    }
    if (index < 0) {
        index = 0;
    }
    if (index >= length) {
        index = length - 1;
    }
    uint8_t scrollBarWidth = 3;
    uint8_t minScrollBarHeight = scrollBarWidth * 2;
    uint8_t scrollBarHeight = bounds[3] / length;
    uint8_t visibleScrollBarHeight = max(scrollBarHeight, minScrollBarHeight);
    this->oled->drawBox(bounds[0] + bounds[2] - scrollBarWidth, bounds[1] + index * scrollBarHeight, scrollBarWidth, visibleScrollBarHeight);
}