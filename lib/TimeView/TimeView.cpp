#include "TimeView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Datetime.h>
#include <bounds.h>

const uint32_t TimeView::multiplier[] = {60, 3600, 86400};

TimeView::TimeView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
    this->focusIndex = 0;
    this->inputIndex = -1;
    this->inputVisible = true;
    this->visibilityDuration = 0;
    this->value = new Datetime(0); // todo destruct
    this->changed = false;
}

void TimeView::mount() {
    this->changed = true;
    this->render();
}

void TimeView::render() {
    if (!this->changed) {
        return;
    }
    this->changed = false;
    OledComponents components(this->oled);
    this->oled->firstPage();
    do {
        uint8_t bounds[4];
        uint8_t focusBounds[4];
        components.setFullScreenBounds(bounds);
        shrinkBounds(bounds, 0, 6);
        bounds[3] = bounds[3] / 2;
        char timeStr[] = "00:00";
        components.drawTextLeft(bounds, "Time");
        this->value->formatLeadingZero(timeStr, this->value->getHours(), 2);
        this->value->formatLeadingZero(timeStr + 3, this->value->getMinutes(), 2);
        components.drawTextRight(bounds, timeStr);
        if (this->focusIndex >=0 && this->focusIndex < 2) {
            copyBounds(bounds, focusBounds);
            if (this->focusIndex == 0) {
                components.setRightTextBounds(focusBounds, timeStr, 0, 2);
            } else {
                components.setRightTextBounds(focusBounds, timeStr, 3, 2);
            }
        }
        
        char dateStr[] = "01.Jan.1970";
        bounds[1] += bounds[3];
        components.drawTextLeft(bounds, "Date");
        this->value->formatLeadingZero(dateStr, this->value->getDay(), 2);
        this->value->formatShortMonth(dateStr + 3, this->value->getMonth());
        this->value->formatLeadingZero(dateStr + 7, this->value->getYear(), 4);
        components.drawTextRight(bounds, dateStr);
        if (this->focusIndex >= 2) {
            copyBounds(bounds, focusBounds);
            if (this->focusIndex == 2) {
                components.setRightTextBounds(focusBounds, dateStr, 0, 2);
            } else if (this->focusIndex == 3) {
                components.setRightTextBounds(focusBounds, dateStr, 3, 3);
            } else {
                components.setRightTextBounds(focusBounds, dateStr, 7, 4);
            }
        }
        if (this->inputIndex > -1 && !this->inputVisible) {
            expandBounds(focusBounds, 1);
        }
        expandBounds(focusBounds, 6, 3);
        components.drawFocus(focusBounds);
    } while (this->oled->nextPage());
}

void TimeView::onClick() {
    if (this->inputIndex > -1) {
        if (this->focusIndex < 4) {
            this->inputIndex++;
            this->focusIndex++;
        } else {
            this->inputIndex = -1;        
        }
    } else {
        this->inputIndex = this->focusIndex;
    }
    this->changed = true;
}

void TimeView::onHold() {
    this->history->back();
}

void TimeView::onInput(bool clockwise) {
    char diff = clockwise ? 1 : -1;
    if (this->inputIndex > -1) {
        if (this->inputIndex == 0) {
            this->value->addSeconds(diff * 3600);
        }
        if (this->inputIndex == 1) {
            this->value->addSeconds(diff * 60);
        }
        
        if (this->inputIndex == 2) {
            this->value->addSeconds(diff * 86400);
        }
        if (this->inputIndex == 3) {
            diff > 0 ? this->value->addMonths(diff) : this->value->subtractMonths(diff * -1);
        }
        if (this->inputIndex == 4) {
            diff > 0 ? this->value->addYears(diff) : this->value->subtractYears(diff * -1);
        }
    } else {
        this->focusIndex += diff;
        if (this->focusIndex < 0) {
            this->focusIndex = 0;
            return;
        }
        if (this->focusIndex > 4) {
            this->focusIndex = 4;
            return;
        }
    }
    this->changed = true;
}

void TimeView::onTick(unsigned long msDiff) {
    if (this->inputIndex > -1) {
        this->visibilityDuration += msDiff;
        if (this->visibilityDuration >= 250) {
            this->inputVisible = !this->inputVisible;
            this->visibilityDuration = 0;
            this->changed = true;
        }
    }
    this->render();
}