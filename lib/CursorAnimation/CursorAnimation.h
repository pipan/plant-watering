#ifndef CursorAnimation_h
#define CursorAnimation_h

class CursorAnimation {
    public:
        CursorAnimation();
        bool tick(int msDiff);
        char getState();
        void toggle();
    private:
        int animationTime;
};

#endif