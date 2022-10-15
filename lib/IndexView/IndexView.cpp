#include "IndexView.h"
#include <ViewHistory.h>
#include <BatteryView.h>
#include <ValvesView.h>
#include <TimeView.h>
#include <U8g2lib.h>
#include <Icons.h>
#include <OledComponents.h>

IndexView::IndexView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
    this->focusIndex = 0;
    this->changed = false;
}

void IndexView::mount() {
    this->changed = true;
    this->render();
}

void IndexView::unmount() {
    this->oled->clearDisplay();
}

void IndexView::onInput(bool clockwise) {
    this->focusIndex += clockwise ? 1 : -1;
    if (this->focusIndex > 2) {
        this->focusIndex = 0;
    }
    if (this->focusIndex < 0) {
        this->focusIndex = 2;
    }
    this->changed = true;
}

void IndexView::onClick() {
    if (this->focusIndex == 0) {
        this->history->pushHistory(new ValvesView(this->oled, this->history));
    } else if (this->focusIndex == 1) {
        this->history->pushHistory(new BatteryView(this->oled, this->history));
    } else if (this->focusIndex == 2) {
        this->history->pushHistory(new TimeView(this->oled, this->history));
    }
}

void IndexView::onTick(unsigned long msDiff) {
    this->render();
}

void IndexView::render() {
    if (!this->changed) {
        return;
    }
    this->changed = false;
    OledComponents components(this->oled);
    this->oled->firstPage();
    const unsigned char *icons[3] = {Icons::valve, Icons::battery, Icons::settings};
    do {
        uint16_t bounds[4];
        components.setFullScreenBounds(bounds);
        bounds[2] = bounds[2] / 3;

        for (int i = 0; i < 3; i++) {
            uint16_t iconBounds[4];
            components.copyBounds(bounds, iconBounds);
            components.setCenterBounds(iconBounds, Icons::width, Icons::height);
            this->oled->drawXBM(iconBounds[0], iconBounds[1], iconBounds[2], iconBounds[3], icons[i]);
            
            if (focusIndex == i) {
                components.drawFocus(iconBounds, 6);
            }
            
            bounds[0] += bounds[2];
        }
    } while (this->oled->nextPage());
}