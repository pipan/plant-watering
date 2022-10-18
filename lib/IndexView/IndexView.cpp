#include "IndexView.h"
#include <ViewHistory.h>
#include <BatteryView.h>
#include <ValvesView.h>
#include <TimeView.h>
#include <U8g2lib.h>
#include <Icons.h>
#include <OledComponents.h>
#include <bounds.h>
#include <Fonts.h>

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

void IndexView::unmount() {}

void IndexView::onInput(bool clockwise) {
    int8_t newFocusIndex = min(max(this->focusIndex + (clockwise ? 1 : -1), 0), 2);
    if (newFocusIndex == this->focusIndex) {
        return;
    }
    this->focusIndex = newFocusIndex;
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
    this->oled->setFont(Fonts::size10);
    this->oled->firstPage();
    const unsigned char *icons[3] = {Icons::valve, Icons::battery, Icons::settings};
    do {
        uint8_t bounds[4];
        components.setFullScreenBounds(bounds);
        components.drawIconsAround(bounds, icons, 3, Icons::width, Icons::height);
        setColumnBounds(bounds, 3, this->focusIndex);
        setCenterBounds(bounds, Icons::width, Icons::height);
        components.drawFocus(bounds, 6);
    } while (this->oled->nextPage());
}