#ifndef EmptyView_h
#define EmptyView_h

#include <View.h>
#include <U8g2lib.h>
#include <ViewHistory.h>

class EmptyView : public View {
    public:
        EmptyView(U8G2 *oled, ViewHistory *history);
        void mount();
        void unmount();
        void onClick();
        void onHold();
        void onInput(bool clockwise);
    private:
        U8G2 *oled;
        ViewHistory *history;
};

#endif