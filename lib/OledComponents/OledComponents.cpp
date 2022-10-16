#include "OledComponents.h"
#include <U8g2lib.h>
#include <bounds.h>

OledComponents::OledComponents(U8G2 *oled) {
    this->oled = oled;
}

void OledComponents::setFullScreenBounds(uint8_t bounds[]) {
    bounds[0] = 0;
    bounds[1] = 0;
    bounds[2] = this->oled->getDisplayWidth();
    bounds[3] = this->oled->getDisplayHeight();
}

void OledComponents::setCenterTextBounds(uint8_t bounds[], const char text[]) {
    uint8_t width = this->oled->getStrWidth(text);
    uint8_t height = this->oled->getFontAscent();
    setCenterBounds(bounds, width, height);
}

void OledComponents::setLeftTextBounds(uint8_t bounds[], const char text[]) {
    uint8_t width = this->oled->getStrWidth(text);
    uint8_t height = this->oled->getFontAscent();
    uint8_t top = (bounds[3] - height) / 2;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::setRightTextBounds(uint8_t bounds[], const char text[]) {
    uint8_t width = this->oled->getStrWidth(text);
    uint8_t height = this->oled->getFontAscent();
    uint8_t top = (bounds[3] - height) / 2;
    bounds[0] = bounds[0] + bounds[2] - width;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::setRightTextBounds(uint8_t bounds[], const char *text, uint8_t start, uint8_t length) {
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
    uint8_t fullWidth = this->oled->getStrWidth(text);
    uint8_t width = this->oled->getStrWidth(subText);
    uint8_t prefixWidth = this->oled->getStrWidth(prefixText);
    uint8_t height = this->oled->getFontAscent();
    uint8_t top = (bounds[3] - height) / 2;
    bounds[0] = bounds[0] + bounds[2] - fullWidth + prefixWidth;
    bounds[1] += top;
    bounds[2] = width;
    bounds[3] = height;
}

void OledComponents::drawTextCenter(uint8_t bounds[], const char text[]) {
    uint8_t textBounds[4];
    copyBounds(bounds, textBounds);
    this->setCenterTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawTextLeft(uint8_t bounds[], const char text[]) {
    uint8_t textBounds[4];
    copyBounds(bounds, textBounds);
    this->setLeftTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawTextRight(uint8_t bounds[], const char text[]) {
    uint8_t textBounds[4];
    copyBounds(bounds, textBounds);
    this->setRightTextBounds(textBounds, text);
    this->oled->drawStr(textBounds[0], textBounds[1] + textBounds[3], text);
}

void OledComponents::drawFocus(const uint8_t bounds[]) {
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

void OledComponents::drawFocus(const uint8_t bounds[], int expand) {
    uint8_t expandedBounds[4];
    copyBounds(bounds, expandedBounds);
    expandBounds(expandedBounds, expand);
    this->drawFocus(expandedBounds);
}

void OledComponents::drawScrollBar(uint8_t bounds[], int8_t length, int8_t index) {
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

void OledComponents::drawIconsAround(uint8_t bounds[], const unsigned char *icons[], uint8_t length, uint8_t width, uint8_t height) {
    uint8_t columnBounds[4];
    copyBounds(bounds, columnBounds);
    setColumnBounds(columnBounds, length, 0);
    for (int i = 0; i < length; i++) {
        uint8_t iconBounds[4];
        copyBounds(columnBounds, iconBounds);
        setCenterBounds(iconBounds, width, height);
        this->oled->drawXBM(iconBounds[0], iconBounds[1], iconBounds[2], iconBounds[3], icons[i]);
        columnBounds[0] += columnBounds[2];
    }
}