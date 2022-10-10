#ifndef BatteryView_h
#define BatteryView_h

#include <View.h>
#include <ViewHistory.h>
#include <U8g2lib.h>

class BatteryView : public View {
    public:
        BatteryView(U8G2 *oled, ViewHistory *history);
        virtual void mount();
        virtual void unmount();
        virtual void onHold();
    private:
        U8G2 *oled;
        ViewHistory *history;
};

#endif