#include "ValvesView.h"
#include <ViewHistory.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Fonts.h>
#include <OledComponents.h>
#include <ValveDetailView.h>

ValvesView::ValvesView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
    this->focusIndex = 0;
    this->scrollIndex = 0;
    this->valves = new char*[8] {"One", "Two", "Three", "Four", "five", "Six", "Seven", "Eight"};
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
    this->focusIndex += clockwise ? 1 : -1;
    if (this->focusIndex < 0) {
        this->focusIndex = 0;
        return;
    }
    if (this->focusIndex > 7) {
        this->focusIndex = 7;
        return;
    }
    if (this->scrollIndex > this->focusIndex) {
        this->scrollIndex = this->focusIndex;
    }
    if (this->scrollIndex + 1 < this->focusIndex) {
        this->scrollIndex = this->focusIndex - 1;
    }
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
    this->oled->setFont(Fonts::size9);
    this->oled->firstPage();
    do {
        uint16_t bounds[4];
        components.setFullScreenBounds(bounds);
        components.drawScrollBar(bounds, 7, this->scrollIndex);
        components.shrinkBounds(bounds, 0, 4);
        components.shrinkBoundsRight(bounds, 8);
        bounds[3] = bounds[3] / 2;
        for (int i = 0; i < 2; i++) {
            components.drawTextLeft(bounds, this->valves[this->scrollIndex + i]);
            if (this->scrollIndex + i == this->focusIndex) {
                uint16_t focusBounds[4];
                components.copyBounds(bounds, focusBounds);
                components.expandBounds(focusBounds, 0, 4);
                components.shrinkBounds(focusBounds, 2, 0);
                components.drawFocus(focusBounds);
            }
            bounds[1] += bounds[3];
        }

    } while (this->oled->nextPage());
}