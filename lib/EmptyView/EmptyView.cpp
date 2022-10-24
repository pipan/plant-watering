#include "EmptyView.h"
#include <ViewHistory.h>
#include <U8g2lib.h>

EmptyView::EmptyView(U8G2 *oled, ViewHistory *history) {
    this->oled = oled;
    this->history = history;
}

void EmptyView::mount() {
    this->oled->firstPage();
    do  {
    } while (this->oled->nextPage());
}

void EmptyView::unmount() {}

void EmptyView::onInput(bool clockwise) {
    this->history->back();
}

void EmptyView::onClick() {
    this->history->back();
}

void EmptyView::onHold() {
    this->history->back();
}