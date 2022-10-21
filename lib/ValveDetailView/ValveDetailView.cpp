#include "ValveDetailView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Icons.h>
#include <bounds.h>

ValveDetailView::ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index) {
    this->oled = oled;
    this->history = history;
    this->index = index;
    this->scrollIndex = 0;
    this->focusIndex = 0;
    this->changed = false;
}

void ValveDetailView::mount() {
    this->changed = true;
}

void ValveDetailView::unmount() { }

void ValveDetailView::onClick() {
    if (this->focusIndex <= 1 || this->focusIndex >= 5) {
        return;
    }
    this->animation.toggle();
    this->changed = true;
}

void ValveDetailView::onHold() {
    this->history->back();
}

void ValveDetailView::onInput(bool clockwise) {
    if (this->animation.getState() > -1) {
        return;
    }
    int8_t newFocusIndex = this->focusIndex + (clockwise ? 1 : -1);
    newFocusIndex = constrain(newFocusIndex, 0, 5);
    if (newFocusIndex == this->focusIndex) {
        return;
    }
    this->focusIndex = newFocusIndex;
    this->changed = true;
    if (this->focusIndex == 0) {
        return;
    }
    this->scrollIndex = constrain(this->scrollIndex, this->focusIndex - 2, this->focusIndex - 1);
}

void ValveDetailView::onTick(unsigned long msDiff) {
    if (this->animation.tick(msDiff)) {
        this->changed = true;
    }
    if (!this->changed) {
        return;
    }
    this->changed = false;
    char expandedPx = this->animation.getState() == 0 ? 8 : 6;
    OledComponents components(this->oled);
    this->oled->firstPage();
    do {
        uint8_t bounds[4];
        components.setFullScreenBounds(bounds);
        components.drawScrollBar(bounds, 4, this->scrollIndex);
        shrinkBounds(bounds, 0, 4);
        shrinkBoundsRight(bounds, 8);
        bounds[3] = bounds[3] / 2;
        if (scrollIndex == 0) {
            const unsigned char *icons[2] = {Icons::run, Icons::calibration};
            components.drawIconsAround(bounds, icons, 2, Icons::width, Icons::height);
            if (focusIndex <= 1) {
                uint8_t focusBounds[4];
                copyBounds(bounds, focusBounds);
                setColumnBounds(focusBounds, 2, focusIndex);
                setCenterBounds(focusBounds, Icons::width, Icons::height);
                components.drawFocus(focusBounds, expandedPx);
            }
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 0 || this->scrollIndex == 1) {
            components.drawInput(bounds, "Repeat", "72h", this->focusIndex == 2 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 1 || this->scrollIndex == 2) {
            components.drawInput(bounds, "Amount", "0.8l", this->focusIndex == 3 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 2 || this->scrollIndex == 3) {
            components.drawInput(bounds, "Split", "1", this->focusIndex == 4 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 3) {
            components.drawInput(bounds, "Auto", "No", this->focusIndex == 5 ? expandedPx : -1);
        }
    } while (this->oled->nextPage());
}