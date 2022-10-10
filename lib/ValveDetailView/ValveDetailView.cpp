#include "ValveDetailView.h"
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <OledComponents.h>
#include <Fonts.h>

ValveDetailView::ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index) {
    this->oled = oled;
    this->history = history;
    this->index = index;
}

void ValveDetailView::mount() {
    OledComponents components(this->oled);
    this->oled->setFont(Fonts::size9);
    this->oled->firstPage();
    do {
        uint16_t bounds[4];
        components.setFullScreenBounds(bounds);
        components.shrinkBounds(bounds, 0, 6);
        bounds[3] = bounds[3] / 2;
        components.drawTextLeft(bounds, "Repeat");
        components.drawTextRight(bounds, "72h");
        bounds[1] += bounds[3];
        components.drawTextLeft(bounds, "Amount");
        components.drawTextRight(bounds, "0.8l");
    } while (this->oled->nextPage());
}

void ValveDetailView::unmount() {
    this->oled->clearDisplay();
}

void ValveDetailView::onHold() {
    this->history->back();
}