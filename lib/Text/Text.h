#ifndef Text_h
#define Text_h

class Text {
    public:
        static int length(char text[]);
        static void append(char buffer[], char text[]);
        static void number(char buffer[], int number, int length);
};

#endif