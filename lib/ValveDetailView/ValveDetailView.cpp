#include "ValveDetailView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Icons.h>
#include <bounds.h>
#include <TimeUnit.h>
#include <VolumeUnit.h>
#include <Text.h>
#include <EEPROM.h>
#include <WatterQueue.h>

ValveDetailView::ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index) {
    this->oled = oled;
    this->history = history;
    this->index = index;
    this->scrollIndex = 0;
    this->focusIndex = 0;
    this->changed = false;
    EEPROM.get(this->index * 64, this->valveConfig);
    if (this->valveConfig.cycle == -1) {
        this->valveConfig.cycle = 48;
    }
    if (this->valveConfig.volume == -1) {
        this->valveConfig.volume = 1000;
    }
    if (this->valveConfig.split == -1) {
        this->valveConfig.split = 1;
    }
    if (this->valveConfig.autoAdjust == -1) {
        this->valveConfig.autoAdjust = 0;
    }
}

void ValveDetailView::mount() {
    this->changed = true;
}

void ValveDetailView::unmount() {
    EEPROM.put(this->index * 64, this->valveConfig);
}

void ValveDetailView::onClick() {
    if (this->focusIndex == 0) {
        WatterQueue::push(this->index);
        return;
    }
    if (this->focusIndex == 1) {
        return;
    }
    if (this->focusIndex == 5) {
        this->valveConfig.autoAdjust = this->valveConfig.autoAdjust ? 0 : 1;
        this->changed = true;
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
            this->valveConfig.cycle= clockwise ? TimeUnit::increment(this->valveConfig.cycle) : TimeUnit::decrement(this->valveConfig.cycle);
            this->valveConfig.cycle = min(this->valveConfig.cycle, 1440);
            this->changed = true;
        }
        if (this->focusIndex == 3) {
            this->valveConfig.volume = clockwise ? VolumeUnit::increment(this->valveConfig.volume) : VolumeUnit::decrement(this->valveConfig.volume);
            this->valveConfig.volume = min(this->valveConfig.volume, 9750);
            this->changed = true;
        }
        if (this->focusIndex == 4) {
            this->valveConfig.split += clockwise ? 1 : -1;
            this->valveConfig.split = constrain(this->valveConfig.split, 0, 9);
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
            const unsigned char *icons[2] = {focusIndex == 0 ? Icons::waterOpen : Icons::waterClosed, Icons::calibration};
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
            Text::number(valueString, this->valveConfig.cycle / 24, 2);
            Text::number(valueString + 4, this->valveConfig.cycle % 24, 2);
            components.drawInput(bounds, "Cycle", valueString, this->focusIndex == 2 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 1 || this->scrollIndex == 2) {
            char valueString[] = "0.00l";
            Text::number(valueString, this->valveConfig.volume / 1000, 1);
            Text::number(valueString + 2, (this->valveConfig.volume % 1000) / 10, 2);
            components.drawInput(bounds, "Amount", valueString, this->focusIndex == 3 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 2 || this->scrollIndex == 3) {
            char valueString[] = "0";
            Text::number(valueString, this->valveConfig.split, 1);
            components.drawInput(bounds, "Split", valueString, this->focusIndex == 4 ? expandedPx : -1);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 3) {
            components.drawInput(bounds, "Auto", this->valveConfig.autoAdjust ? "Yes" : "No", this->focusIndex == 5 ? expandedPx : -1);
        }
    } while (this->oled->nextPage());
}