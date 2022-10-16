#include "BatteryView.h"
#include <ViewHistory.h>
#include <U8g2lib.h>
#include <OledComponents.h>
#include <bounds.h>

BatteryView::BatteryView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
}

void BatteryView::mount() {
    OledComponents components(this->oled);
    this->oled->firstPage();
    do {
        uint8_t bounds[4];
        components.setFullScreenBounds(bounds);
        shrinkBounds(bounds, 0, 6);
        bounds[3] = bounds[3] / 2;
        components.drawTextLeft(bounds, "Charge");
        components.drawTextRight(bounds, "63%");
    } while (this->oled->nextPage());
}

void BatteryView::unmount() {
    this->oled->clearDisplay();
}

void BatteryView::onHold() {
    this->history->back();
}