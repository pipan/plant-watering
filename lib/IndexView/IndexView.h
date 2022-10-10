#ifndef IndexView_h
#define IndexView_h

#include <View.h>
#include <U8g2lib.h>
#include <ViewHistory.h>

class IndexView : public View {
    public:
        IndexView(U8G2 *oled, ViewHistory *history);
        void mount();
        void unmount();
        void onClick();
        void onInput(bool clockwise);
        void onTick(unsigned long msDiff);
    private:
        U8G2 *oled;
        ViewHistory *history;
        int8_t focusIndex;
        bool changed;
        void render();
};

#endif