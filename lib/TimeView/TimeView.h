#ifndef TimeView_h
#define TimeView_h

#include <View.h>
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <CursorAnimation.h>

class TimeView : public View {
    public:
        TimeView(U8G2 *oled, ViewHistory *history);
        void mount();
        void onClick();
        void onHold();
        void onInput(bool clockwise);
        void onTick(unsigned long msDiff);
    private:
        U8G2 *oled;
        ViewHistory *history;
        int8_t focusIndex;
        CursorAnimation animation;
        char datetime[5];
        char inputValue;
        int refreshTimeBuffer;
        bool changed;
        void render();
};

#endif