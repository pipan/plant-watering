#include "Datetime.h"
#include "../Time/Time.h"
#include <Arduino.h>

const long Datetime::MINUTE_IN_SECONDS = 60;
const long Datetime::HOUR_IN_SECONDS = 3600;
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
    unsigned long hoursRemainder = this->timestamp % Datetime::DAY_IN_SECONDS;
    return hoursRemainder / Datetime::HOUR_IN_SECONDS;
}

unsigned char Datetime::getDay() {
    int year;
    long reminder;
    this->setYearAndReminder(&year, &reminder);
    int dayInYear = reminder / Datetime::DAY_IN_SECONDS + 1;
    for (int i = 0; i < 12; i++) {
        unsigned char daysInMonth = Time::getDaysInMonth(year, i);
        if (dayInYear <= daysInMonth) {
            return dayInYear;
        }
        dayInYear -= daysInMonth;
    }
    return dayInYear;
}

unsigned char Datetime::getMonth() {
    int year;
    long reminder;
    this->setYearAndReminder(&year, &reminder);
    int dayInYear = reminder / Datetime::DAY_IN_SECONDS + 1;
    for (int i = 0; i < 12; i++) {
        unsigned char daysInMonth = Time::getDaysInMonth(year, i);
        if (dayInYear <= daysInMonth) {
            return i;
        }
        dayInYear -= daysInMonth;
    }
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

void Datetime::addMonths(int months) {
    if (months < 0) {
        return this->subtractMonths(months * -1);
    }
    int year;
    long reminder;
    for (int i = 0; i < months; i++) {
        unsigned char month = this->getMonth();
        unsigned char day = this->getDay();
        this->setYearAndReminder(&year, &reminder);
        char nextMonth = month + 1 >= 12 ? 0 : month + 1;
        unsigned char currentDaysInMonth = Time::getDaysInMonth(year, month);
        unsigned char nextDaysInMonth = Time::getDaysInMonth(year, nextMonth);
        this->addSeconds(Datetime::DAY_IN_SECONDS * (currentDaysInMonth - day + 1));
        this->addSeconds(Datetime::DAY_IN_SECONDS * min(day - 1, nextDaysInMonth - 1));
    }
}

void Datetime::addYears(int years) {
    if (years < 0) {
        return this->subtractYears(years * -1);
    }
    int year = this->getYear();
    unsigned char month = this->getMonth();
    for (int i = 0; i < years; i++) {
        if (Time::isLeapYear(year) && month < 2) {
            this->addSeconds(Datetime::DAY_IN_SECONDS);
        }
        this->addSeconds(Datetime::YEAR_IN_SECONDS);
        year++;
        if (Time::isLeapYear(year) && month >= 2) {
            this->addSeconds(Datetime::DAY_IN_SECONDS);
        }
    }
}

void Datetime::formatLeadingZero(char *buffer, int value, int length) {
    for (int i = length - 1; i >= 0; i--) {
        buffer[i] = 48 + (value % 10);
        value = value / 10;
    }
}

void Datetime::formatShortMonth(char *buffer, int month) {
    const char *monthShort = Time::getMonthShortStr(month);
    for (int i = 0; i < 3; i++) {
        buffer[i] = monthShort[i];
    }
}

void Datetime::subtractMonths(int months) {
    if (months < 0) {
        return this->addMonths(months * -1);
    }
    int year;
    long reminder;
    for (int i = 0; i < months; i++) {
        unsigned char month = this->getMonth();
        unsigned char day = this->getDay();
        this->setYearAndReminder(&year, &reminder);
        char nextMonth = month - 1 < 0 ? 11 : month - 1;
        unsigned char currentDaysInMonth = Time::getDaysInMonth(year, month);
        unsigned char nextDaysInMonth = Time::getDaysInMonth(year, nextMonth);
        this->addSeconds(Datetime::DAY_IN_SECONDS * day * -1);
        this->addSeconds(Datetime::DAY_IN_SECONDS * (nextDaysInMonth - min(day, nextDaysInMonth)) * -1);
    }
}

void Datetime::subtractYears(int years) {
    if (years < 0) {
        return this->addYears(years * -1);
    }
    int year = this->getYear();
    unsigned char month = this->getMonth();
    for (int i = 0; i < years; i++) {
        if (Time::isLeapYear(year) && month >= 2) {
            this->addSeconds(Datetime::DAY_IN_SECONDS * -1);
        }
        this->addSeconds((Datetime::YEAR_IN_SECONDS) * -1);
        year--;
        if (Time::isLeapYear(year) && month < 2) {
            this->addSeconds(Datetime::DAY_IN_SECONDS * -1);
        }
    }
}

unsigned long Datetime::getYearInSeconds(int year) {
    unsigned long secondsInYear = Datetime::YEAR_IN_SECONDS;
    if (Time::isLeapYear(year)) {
        secondsInYear += Datetime::DAY_IN_SECONDS;
    }
    return secondsInYear;
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