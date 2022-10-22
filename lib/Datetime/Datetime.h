#ifndef Datetime_h
#define Datetime_h

class Datetime {
    public:
        static void formatLeadingZero(char *buffer, int value, int length);
        static void formatShortMonth(char *buffer, int month);
        static unsigned long msDiff(unsigned long before, unsigned long after);
        static bool isLeapYear(int year);
        static unsigned char getDaysInMonth(int year, unsigned char month);
        static const char* getMonthShortStr(unsigned char month);
    private:
        static const unsigned char monthDays[];
        static const char *monthShortStr[];
};

#endif