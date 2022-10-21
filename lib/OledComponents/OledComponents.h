#ifndef OledComponents_h
#define OledComponents_h

#include <U8g2lib.h>

class OledComponents {
    public:
        OledComponents(U8G2 *oled);
        void setFullScreenBounds(uint8_t bounds[]);
        void setCenterTextBounds(uint8_t bounds[], const char text[]);
        void setLeftTextBounds(uint8_t bounds[], const char text[]);
        void setRightTextBounds(uint8_t bounds[], const char text[]);
        void setRightTextBounds(uint8_t bounds[], const char text[], uint8_t start, uint8_t length);
        void drawScrollBar(uint8_t bounds[], int8_t length, int8_t index);
        void drawTextCenter(uint8_t bounds[], const char text[]);
        void drawTextLeft(uint8_t bounds[], const char text[]);
        void drawTextRight(uint8_t bounds[], const char text[]);
        void drawInput(uint8_t bounds[], const char label[], const char value[], int8_t expandedFocus);
        void drawFocus(const uint8_t bounds[]);
        void drawFocus(const uint8_t bounds[], int expand);
        void drawIconsAround(uint8_t bounds[], const unsigned char *icons[], uint8_t length, uint8_t width, uint8_t height);
    private:
        U8G2 *oled;
};

#endif