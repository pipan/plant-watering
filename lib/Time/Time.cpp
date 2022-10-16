#include "Time.h"

const unsigned char Time::monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char* Time::monthShortStr[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

unsigned long Time::msDiff(unsigned long before, unsigned long after) {
    if (after < before) {
        return after + (0xFFFFFFFFFFFFFFFF - before);
    }
    return after - before;
}

bool Time::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

unsigned char Time::getDaysInMonth(int year, unsigned char month) {
    if (month == 1 && Time::isLeapYear(year)) {
        return 29;
    }
    return Time::monthDays[month];
}

const char* Time::getMonthShortStr(unsigned char month) {
    return Time::monthShortStr[month];
}