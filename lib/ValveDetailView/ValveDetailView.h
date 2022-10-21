#ifndef ValveDetailView_h
#define ValveDetailView_h

#include <View.h>
#include <ViewHistory.h>
#include <U8g2lib.h>
#include <CursorAnimation.h>

class ValveDetailView : public View {
    public:
        ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index);
        virtual void mount();
        virtual void unmount();
        virtual void onClick();
        virtual void onHold();
        virtual void onInput(bool clockwise);
        virtual void onTick(unsigned long msDiff);
    private:
        U8G2 *oled;
        ViewHistory *history;
        unsigned char index;
        char scrollIndex;
        char focusIndex;
        bool changed;
        CursorAnimation animation;
};

#endif