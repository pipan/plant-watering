#ifndef Time_h
#define Time_h

class Time {
    public:
        static unsigned long msDiff(unsigned long before, unsigned long after);
        static bool isLeapYear(int year);
        static unsigned char getDaysInMonth(int year, unsigned char month);
        static const char* getMonthShortStr(unsigned char month);
    private:
        static const unsigned char monthDays[];
        static const char *monthShortStr[];
};

#endif