#ifndef OledComponents_h
#define OledComponents_h

#include <U8g2lib.h>

class OledComponents {
    public:
        OledComponents(U8G2 *oled);
        void copyBounds(uint16_t *source, uint16_t *destination);
        void setFullScreenBounds(uint16_t *bounds);
        void setCenterTextBounds(uint16_t bounds[], char *text);
        void setCenterBounds(uint16_t bounds[], uint16_t width, uint16_t height);
        void setLeftTextBounds(uint16_t bounds[], char *text);
        void setRightTextBounds(uint16_t bounds[], char *text);
        void setRightTextBounds(uint16_t bounds[], char *text, uint8_t start, uint8_t length);
        void expandBounds(uint16_t *bounds, int8_t size);
        void expandBounds(uint16_t *bounds, int8_t vertical, int8_t horizontal);
        void shrinkBounds(uint16_t *bounds, int8_t size);
        void shrinkBounds(uint16_t *bounds, int8_t vertical, int8_t horizontal);
        void shrinkBoundsRight(uint16_t *bounds, int8_t value);
        void drawScrollBar(uint16_t *bounds, int8_t length, int8_t index);
        void drawTextCenter(uint16_t bounds[], char *text);
        void drawTextLeft(uint16_t bounds[], char *text);
        void drawTextRight(uint16_t bounds[], char *text);
        void drawFocus(uint16_t bounds[]);
    private:
        U8G2 *oled;
};

#endif