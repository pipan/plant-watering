#ifndef ViewController_h
#define ViewController_h

#include <ViewHistory.h>
#include <View.h>

class ViewController : public ViewHistory {
    public:
        ViewController();
        void pushHistory(View *view);
        void back();
        void onClick();
        void onHold();
        void onInput(bool clockwise);
        void onTick(unsigned long msDiff);
    private:
        View *historyStack[4];
        unsigned char stackLength;
};

#endif