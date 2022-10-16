#ifndef TimeView_h
#define TimeView_h

#include <View.h>
#include <U8g2lib.h>
#include <ViewHistory.h>
#include <Datetime.h>

class TimeView : public View {
    public:
        TimeView(U8G2 *oled, ViewHistory *history);
        void mount();
        void onClick();
        void onHold();
        void onInput(bool clockwise);
        void onTick(unsigned long msDiff);
    private:
        static const uint32_t multiplier[];
        U8G2 *oled;
        ViewHistory *history;
        int8_t focusIndex;
        int8_t inputIndex;
        Datetime *value;
        uint16_t visibilityDuration;
        bool inputVisible;
        bool changed;
        void render();
};

#endif