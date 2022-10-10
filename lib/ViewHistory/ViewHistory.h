#ifndef ViewHistory_h
#define ViewHistory_h

#include <View.h>

class ViewHistory {
    public:
        virtual void pushHistory(View *view) = 0;
        virtual void back() = 0;
};

#endif