#include "ViewController.h"
#include <View.h>

ViewController::ViewController() {
    this->stackLength = 0;
}

void ViewController::pushHistory(View *view) {
    if (view == 0) {
        return;
    }
    if (this->stackLength > 0) {
        this->historyStack[this->stackLength - 1]->unmount();
    }
    this->historyStack[this->stackLength] = view;
    this->stackLength++;
    view->mount();
}

void ViewController::back() {
    if (this->stackLength <= 1) {
        return;
    }
    this->historyStack[this->stackLength - 1]->unmount();
    delete [] this->historyStack[this->stackLength - 1];
    this->stackLength--;
    this->historyStack[this->stackLength - 1]->mount();
}

void ViewController::home() {
    if (this->stackLength <= 1) {
        return;
    }
    do {
        this->historyStack[this->stackLength - 1]->unmount();
        delete [] this->historyStack[this->stackLength - 1];
        this->stackLength--;
    } while (this->stackLength > 1);
    this->historyStack[this->stackLength - 1]->mount();
}

void ViewController::onClick() {
    if (this->stackLength <= 0) {
        return;
    }
    this->historyStack[this->stackLength - 1]->onClick();
}

void ViewController::onHold() {
    if (this->stackLength <= 0) {
        return;
    }
    this->historyStack[this->stackLength - 1]->onHold();
}

void ViewController::onInput(bool clockwise) {
    if (this->stackLength <= 0) {
        return;
    }
    this->historyStack[this->stackLength - 1]->onInput(clockwise);
}

void ViewController::onTick(unsigned long msDiff) {
    if (this->stackLength <= 0) {
        return;
    }
    this->historyStack[this->stackLength - 1]->onTick(msDiff);
}
