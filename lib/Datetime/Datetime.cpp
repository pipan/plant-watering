#include "Datetime.h"
#include <Time.h>
#include <Arduino.h>

const char Datetime::MINUTE_IN_SECONDS = 60;
const int Datetime::HOUR_IN_SECONDS = 3600;
const long Datetime::DAY_IN_SECONDS = 86400;
const long Datetime::YEAR_IN_SECONDS = 31536000;

Datetime::Datetime(long long timestamp) {
    this->setTimestamp(timestamp);
}

long long Datetime::getTimestamp() {
    return this->timestamp;
}

void Datetime::setTimestamp(long long timestamp) {
    if (timestamp < 0) {
        timestamp = 0;
    }
    this->timestamp = timestamp;
}

unsigned char Datetime::getMinutes() {
    return (this->timestamp % Datetime::HOUR_IN_SECONDS) / Datetime::MINUTE_IN_SECONDS;
}

unsigned char Datetime::getHours() {
    return (this->timestamp % Datetime::DAY_IN_SECONDS) / Datetime::HOUR_IN_SECONDS;
}

unsigned char Datetime::getDay() {
    int year;
    long reminder;
    this->setYearAndReminder(&year, &reminder);
    int dayInYear = reminder / Datetime::DAY_IN_SECONDS + 1;
    char i = 0;
    do {
        unsigned char daysInMonth = Time::getDaysInMonth(year, i++);
        if (dayInYear <= daysInMonth) {
            return dayInYear;
        }
        dayInYear -= daysInMonth;
    } while (dayInYear);
    return 0;
}

unsigned char Datetime::getMonth() {
    int year;
    long reminder;
    this->setYearAndReminder(&year, &reminder);
    int dayInYear = reminder / Datetime::DAY_IN_SECONDS + 1;
    char i = 0;
    do {
        unsigned char daysInMonth = Time::getDaysInMonth(year, i);
        if (dayInYear <= daysInMonth) {
            return i;
        }
        dayInYear -= daysInMonth;
        i++;
    } while (dayInYear);
    return 0;
}

int Datetime::getYear() {
    int year;
    long reminder;
    this->setYearAndReminder(&year, &reminder);
    return year;
}

void Datetime::addSeconds(long long seconds) {
    this->setTimestamp(this->timestamp + seconds);
}

void Datetime::addMonths(unsigned char months) {
    int year;
    long reminder;
    while (months > 0) {
        unsigned char month = this->getMonth();
        unsigned char day = this->getDay();
        this->setYearAndReminder(&year, &reminder);
        char nextMonth = month + 1 >= 12 ? 0 : month + 1;
        unsigned char currentDaysInMonth = Time::getDaysInMonth(year, month);
        unsigned char nextDaysInMonth = Time::getDaysInMonth(year, nextMonth);
        this->addSeconds(Datetime::DAY_IN_SECONDS * (currentDaysInMonth - day + 1));
        this->addSeconds(Datetime::DAY_IN_SECONDS * min(day - 1, nextDaysInMonth - 1));
        months--;
    }
}

void Datetime::addYears(unsigned char years) {
    int year = this->getYear();
    unsigned char month = this->getMonth();
    while (years > 0) {
        long addSeconds = Datetime::YEAR_IN_SECONDS;
        if (Time::isLeapYear(year) && month < 2) {
            addSeconds += Datetime::DAY_IN_SECONDS;
        }
        year++;
        if (Time::isLeapYear(year) && month >= 2) {
            addSeconds += Datetime::DAY_IN_SECONDS;
        }
        this->addSeconds(addSeconds);
        years--;
    }
}

void Datetime::formatLeadingZero(char *buffer, int value, int length) {
    while (length > 0) {
        buffer[--length] = 48 + (value % 10);
        value = value / 10;
    }
}

void Datetime::formatShortMonth(char *buffer, int month) {
    const char *monthShort = Time::getMonthShortStr(month);
    for (int i = 2; i >= 0; i--) {
        buffer[i] = monthShort[i];
    }
}

void Datetime::subtractMonths(unsigned char months) {
    int year;
    long reminder;
    while (months > 0) {
        unsigned char month = this->getMonth();
        unsigned char day = this->getDay();
        this->setYearAndReminder(&year, &reminder);
        char nextMonth = month - 1 < 0 ? 11 : month - 1;
        unsigned char nextDaysInMonth = Time::getDaysInMonth(year, nextMonth);
        this->addSeconds(Datetime::DAY_IN_SECONDS * day * -1);
        this->addSeconds(Datetime::DAY_IN_SECONDS * (nextDaysInMonth - min(day, nextDaysInMonth)) * -1);
        months--;
    }
}

void Datetime::subtractYears(unsigned char years) {
    int year = this->getYear();
    unsigned char month = this->getMonth();
    while (years > 0) {
        long subtractSeconds = Datetime::YEAR_IN_SECONDS;
        if (Time::isLeapYear(year) && month >= 2) {
            subtractSeconds += Datetime::DAY_IN_SECONDS;
        }
        year--;
        if (Time::isLeapYear(year) && month < 2) {
            subtractSeconds += Datetime::DAY_IN_SECONDS;
        }
        this->addSeconds(subtractSeconds * -1);
        years--;
    }
}

unsigned long Datetime::getYearInSeconds(int year) {
    if (Time::isLeapYear(year)) {
        return Datetime::YEAR_IN_SECONDS + Datetime::DAY_IN_SECONDS;
    }
    return Datetime::YEAR_IN_SECONDS;
}

void Datetime::setYearAndReminder(int *year, long *reminder) {
    if (this->timestamp < 0) {
        *year = 1970;
        if (reminder != 0) {
            *reminder = 0;
        }
        return;
    }
    long long timestampRemainder = this->timestamp;
    *year = 1970;
    while (timestampRemainder >= 0) {
        long secondsInYear = this->getYearInSeconds(*year);
        if (timestampRemainder < secondsInYear) {
            if (reminder != 0) {
                *reminder = timestampRemainder;
            }
            return;
        }
        *year += 1;
        timestampRemainder -= secondsInYear;
    }
    if (reminder != 0) {
        *reminder = 0;
    }
    return;
}