#include "Datetime.h"
#include <Arduino.h>

const unsigned char Datetime::monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char* Datetime::monthShortStr[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

unsigned long Datetime::msDiff(unsigned long before, unsigned long after) {
    if (after < before) {
        return after + (0xFFFFFFFFFFFFFFFF - before);
    }
    return after - before;
}

bool Datetime::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

unsigned char Datetime::getDaysInMonth(int year, unsigned char month) {
    if (month == 1 && Datetime::isLeapYear(year)) {
        return 29;
    }
    return Datetime::monthDays[month];
}

const char* Datetime::getMonthShortStr(unsigned char month) {
    return Datetime::monthShortStr[month];
}

void Datetime::formatLeadingZero(char *buffer, int value, int length) {
    while (length > 0) {
        buffer[--length] = 48 + (value % 10);
        value = value / 10;
    }
}

void Datetime::formatShortMonth(char *buffer, int month) {
    const char *monthShort = Datetime::getMonthShortStr(month);
    for (int i = 2; i >= 0; i--) {
        buffer[i] = monthShort[i];
    }
}