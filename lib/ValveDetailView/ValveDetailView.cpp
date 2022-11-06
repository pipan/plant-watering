#include "ValveDetailView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Icons.h>
#include <bounds.h>
#include <TimeUnit.h>
#include <VolumeUnit.h>
#include <Text.h>

ValveDetailView::ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index) {
    this->oled = oled;
    this->history = history;
    this->index = index;
    this->scrollIndex = 0;
    this->focusIndex = 0;
    this->changed = false;
    this->repeatValue = 0;
    this->volumeValue = 0;
    this->splitValue = 0;
    this->autoValue = false;
}

void ValveDetailView::mount() {
    this->changed = true;
}

void ValveDetailView::unmount() { }

void ValveDetailView::onClick() {
    if (this->focusIndex <= 1 || this->focusIndex >= 5) {
        if (this->focusIndex == 5) {
            this->autoValue = !this->autoValue;
            this->changed = true;
        }
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
        if (this->focusIndex == 2) {
            this->repeatValue = clockwise ? TimeUnit::increment(this->repeatValue) : TimeUnit::decrement(this->repeatValue);
            this->repeatValue = min(this->repeatValue, 1440);
            this->changed = true;
        }
        if (this->focusIndex == 3) {
            this->volumeValue = clockwise ? VolumeUnit::increment(this->volumeValue) : VolumeUnit::decrement(this->volumeValue);
            this->volumeValue = min(this->volumeValue, 9750);
            this->changed = true;
        }
        if (this->focusIndex == 4) {
            this->splitValue += clockwise ? 1 : -1;
            this->splitValue = constrain(this->splitValue, 0, 9);
            this->changed = true;
        }
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
            char valueString[] = "00d 00h";
            Text::number(valueString, this->repeatValue / 24, 2);
            Text::number(valueString + 4, this->repeatValue % 24, 2);
            components.drawInput(bounds, "Repeat", valueString, this->focusIndex == 2 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 1 || this->scrollIndex == 2) {
            char valueString[] = "0.00l";
            Text::number(valueString, this->volumeValue / 1000, 1);
            Text::number(valueString + 2, (this->volumeValue % 1000) / 10, 2);
            components.drawInput(bounds, "Amount", valueString, this->focusIndex == 3 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 2 || this->scrollIndex == 3) {
            char valueString[] = "0";
            Text::number(valueString, this->splitValue, 1);
            components.drawInput(bounds, "Split", valueString, this->focusIndex == 4 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 3) {
            components.drawInput(bounds, "Auto", this->autoValue ? "Yes" : "No", this->focusIndex == 5 ? expandedPx : -1);
        }
    } while (this->oled->nextPage());
}