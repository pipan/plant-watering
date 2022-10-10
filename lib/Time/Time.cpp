#include "Time.h"

const unsigned char Time::monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char** Time::monthShortStr = new char*[12] {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

unsigned long Time::msDiff(unsigned long before, unsigned long after) {
    unsigned long diff = after - before;
    if (diff < 0) {
        diff = after + (0xFFFFFFF - before);
    }
    return diff;
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