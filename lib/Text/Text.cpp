#include "Text.h"

int Text::length(char text[]) {
    int i = 0;
    while (text[i] != '\0') {
        i++;
    }
    return i;
}

void Text::append(char buffer[], char text[]) {
    int bufferIndex = Text::length(buffer);
    int i = 0;
    while (text[i] != '\0') {
        buffer[bufferIndex + i] = text[i];
        i++;
    }
    buffer[bufferIndex + i] = '\0';
}

void Text::number(char buffer[], int number, int length) {
    while (length > 0) {
        buffer[--length] = 48 + (number % 10);
        number = number / 10;
    }
}