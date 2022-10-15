#include "ValveDetailView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Fonts.h>
#include <Icons.h>

ValveDetailView::ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index) {
    this->oled = oled;
    this->history = history;
    this->index = index;
    this->scrollIndex = 0;
    this->changed = false;
}

void ValveDetailView::mount() {
    this->changed = true;
}

void ValveDetailView::unmount() {
    this->oled->clearDisplay();
}

void ValveDetailView::onHold() {
    this->history->back();
}

void ValveDetailView::onInput(bool clockwise) {
    this->scrollIndex += clockwise ? 1 : -1;
    if (this->scrollIndex < 0) {
        this->scrollIndex = 0;
        return;
    }
    if (this->scrollIndex > 1) {
        this->scrollIndex = 1;
        return;
    }
    this->changed = true;
}

void ValveDetailView::onTick(unsigned long msDiff) {
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
        components.drawScrollBar(bounds, 2, this->scrollIndex);
        components.shrinkBounds(bounds, 0, 4);
        components.shrinkBoundsRight(bounds, 8);
        bounds[3] = bounds[3] / 2;
        if (this->scrollIndex == 0) {
            const unsigned char *icons[2] = {Icons::run, Icons::calibration};
            components.drawIconsAround(bounds, icons, 2, Icons::width, Icons::height);
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 0 || this->scrollIndex == 1) {
            components.drawTextLeft(bounds, "Repeat");
            components.drawTextRight(bounds, "72h");
            bounds[1] += bounds[3];
        }
        if (this->scrollIndex == 1) {
            components.drawTextLeft(bounds, "Amount");
            components.drawTextRight(bounds, "0.8l");
            bounds[1] += bounds[3];
        }        
    } while (this->oled->nextPage());
}