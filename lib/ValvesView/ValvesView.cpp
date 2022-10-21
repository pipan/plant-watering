#include "ValvesView.h"
#include <ViewHistory.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <OledComponents.h>
#include <ValveDetailView.h>
#include <bounds.h>

ValvesView::ValvesView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
    this->focusIndex = 0;
    this->changed = false;
}

void ValvesView::mount() {
    this->changed = true;
    this->render();
}

void ValvesView::unmount() {
    this->oled->clearDisplay();
}

void ValvesView::onHold() {
    this->history->back();
}

void ValvesView::onClick() {
    this->history->pushHistory(new ValveDetailView(this->oled, this->history, this->focusIndex));
}

void ValvesView::onInput(bool clockwise) {
    int8_t newIndex = this->focusIndex + (clockwise ? 1 : -1);
    newIndex = constrain(newIndex, 0, 7);
    if (newIndex == this->focusIndex) {
        return;
    }
    this->focusIndex = newIndex;
    this->changed = true;
}

void ValvesView::onTick(unsigned long msDiff) {
    this->render();
}

void ValvesView::render() {
    if (!this->changed) {
        return;
    }
    this->changed = false;
    OledComponents components(this->oled);
    this->oled->firstPage();
    do {
        uint8_t bounds[4];
        components.setFullScreenBounds(bounds);
        for (int i = 0; i < 2; i++) {
            uint8_t rowBounds[4];
            copyBounds(bounds, rowBounds);
            setRowBounds(rowBounds, 2, i);
            for (int j = 0; j < 8; j++) {
                uint8_t cellBounds[4];
                copyBounds(rowBounds, cellBounds);
                setColumnBounds(cellBounds, 4, j);
                char numberChar = 48 + i * 4 + j + 1;
                char numberStr[2] = {numberChar, '\0'};
                components.drawTextCenter(cellBounds, numberStr);
                if (this->focusIndex == i * 4 + j) {
                    components.setCenterTextBounds(cellBounds, numberStr);
                    components.drawFocus(cellBounds, 6);
                }
            }
        }
    } while (this->oled->nextPage());
}