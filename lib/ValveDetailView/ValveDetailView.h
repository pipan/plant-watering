#ifndef ValveDetailView_h
#define ValveDetailView_h

#include <View.h>
#include <ViewHistory.h>
#include <U8g2lib.h>

class ValveDetailView : public View {
    public:
        ValveDetailView(U8G2 *oled, ViewHistory *history, unsigned char index);
        virtual void mount();
        virtual void unmount();
        virtual void onHold();
    private:
        U8G2 *oled;
        ViewHistory *history;
        unsigned char index;
};

#endif