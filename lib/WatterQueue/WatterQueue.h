#ifndef WatterQueue_h
#define WatterQueue_h

#define WATTER_QUEUE_SIZE 16

class WatterQueue {
    public:
        static void push(char valveIndex);
        static char pop();
        static bool contains(char valveIndex);
    private:
        static char queue[WATTER_QUEUE_SIZE];
        static unsigned char size;
};

#endif