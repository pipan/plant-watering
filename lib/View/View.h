#ifndef View_h
#define View_h

class View {
    public:
        virtual void mount();
        virtual void unmount();
        virtual void onClick();
        virtual void onHold();
        virtual void onInput(bool clockwise);
        virtual void onTick(unsigned long msDiff);
};

#endif