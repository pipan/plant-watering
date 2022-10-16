#ifndef ValvesView_h
#define ValvesView_h

#include <View.h>
#include <ViewHistory.h>
#include <U8g2lib.h>

class ValvesView : public View {
    public:
        ValvesView(U8G2 *oled, ViewHistory *history);
        virtual void mount();
        virtual void unmount();
        virtual void onHold();
        virtual void onClick();
        virtual void onInput(bool clockwise);
        virtual void onTick(unsigned long msDiff);
    private:
        int8_t focusIndex;
        uint8_t scrollIndex;
        bool changed;
        U8G2 *oled;
        ViewHistory *history;
        void render();
};

#endif