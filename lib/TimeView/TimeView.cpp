#include "TimeView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Datetime.h>
#include <bounds.h>
#include <Rtc.h>
#include <Datetime.h>
#include <Number.h>

TimeView::TimeView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
    this->focusIndex = 0;
    this->inputValue = 0;
    this->changed = false;
    this->refreshTimeBuffer = 0;
}

void TimeView::mount() {
    this->changed = true;
    this->datetime[0] = -1;
}

void TimeView::render() {
    if (!this->changed) {
        return;
    }
    this->changed = false;
    if (this->animation.getState() > -1) {
        datetime[this->focusIndex] = this->inputValue;
    }
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
        Datetime::formatLeadingZero(timeStr, this->datetime[0], 2);
        Datetime::formatLeadingZero(timeStr + 3, this->datetime[1], 2);
        components.drawTextRight(bounds, timeStr);
        if (this->focusIndex >=0 && this->focusIndex < 2) {
            copyBounds(bounds, focusBounds);
            if (this->focusIndex == 0) {
                components.setRightTextBounds(focusBounds, timeStr, 0, 2);
            } else {
                components.setRightTextBounds(focusBounds, timeStr, 3, 2);
            }
        }
        
        char dateStr[] = "01.Jan.2000";
        bounds[1] += bounds[3];
        components.drawTextLeft(bounds, "Date");
        Datetime::formatLeadingZero(dateStr, this->datetime[2], 2);
        Datetime::formatShortMonth(dateStr + 3, this->datetime[3]);
        Datetime::formatLeadingZero(dateStr + 7, this->datetime[4] + 2000, 4);
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
        if (this->animation.getState() == 0) {
            expandBounds(focusBounds, 1);
        }
        expandBounds(focusBounds, 6, 3);
        components.drawFocus(focusBounds);
    } while (this->oled->nextPage());
}

void TimeView::onClick() {
    if (this->animation.getState() == -1) {
        this->inputValue = this->datetime[this->focusIndex];
    } else {
        if (this->focusIndex == 0) {
            Rtc::setHours(this->inputValue);
        }
        if (this->focusIndex == 1) {
            Rtc::setMinutes(this->inputValue);
        }
        
        if (this->focusIndex == 2) {
            Rtc::setDate(this->inputValue);
        }
        if (this->focusIndex == 3) {
            Rtc::setMonth(this->inputValue + 1);
            char maxDate = Datetime::getDaysInMonth(this->datetime[4] + 2000, this->inputValue);
            if (maxDate < this->datetime[2]) {
                Rtc::setDate(maxDate);
            }
        }
        if (this->focusIndex == 4) {
            Rtc::setYear(this->inputValue + 2000);
            char maxDate = Datetime::getDaysInMonth(this->inputValue + 2000, this->datetime[3]);
            if (maxDate < this->datetime[2]) {
                Rtc::setDate(maxDate);
            }
        }
        this->inputValue = 0;
        this->datetime[0] = -1;
    }
    this->animation.toggle();
    this->changed = true;
}

void TimeView::onHold() {
    this->history->back();
}

void TimeView::onInput(bool clockwise) {
    char diff = clockwise ? 1 : -1;
    if (this->animation.getState() > -1) {
        this->inputValue += diff;
        if (this->focusIndex == 0) {
            this->inputValue = constrainCycle(this->inputValue, 0, 23);
        }
        if (this->focusIndex == 1) {
            this->inputValue = constrainCycle(this->inputValue, 0, 59);
        }

        if (this->focusIndex == 2) {
            char maxDate = Datetime::getDaysInMonth(this->datetime[4] + 2000, this->datetime[3]);
            this->inputValue = constrainCycle(this->inputValue, 1, maxDate);
        }
        if (this->focusIndex == 3) {
            this->inputValue = constrainCycle(this->inputValue, 0, 11);
        }
        if (this->focusIndex == 4) {
            this->inputValue = constrainCycle(this->inputValue, 0, 99);
        }
    } else {
        int8_t newFocus = this->focusIndex + diff;
        newFocus = constrain(newFocus, 0, 4);
        if (newFocus == this->focusIndex) {
            return;
        }
        this->focusIndex = newFocus;
    }
    this->changed = true;
}

void TimeView::onTick(unsigned long msDiff) {
    if (this->datetime[0] < 0) {
        this->datetime[0] = Rtc::getHours();
        this->datetime[1] = Rtc::getMinutes();
        this->datetime[2] = Rtc::getDate();
        this->datetime[3] = Rtc::getMonth() - 1;
        this->datetime[4] = Rtc::getYearChar();
        this->changed = true;
    }
    this->refreshTimeBuffer += msDiff;
    if (this->refreshTimeBuffer > 10000) {
        this->refreshTimeBuffer = 0;
        this->changed = true;
    }
    if (this->animation.getState() > -1) {
        if (this->animation.tick(msDiff)) {
            this->changed = true;
        }
    }
    this->render();
}