#ifndef Datetime_h
#define Datetime_h

class Datetime {
    public:
        static const long MINUTE_IN_SECONDS;
        static const long HOUR_IN_SECONDS;
        static const long DAY_IN_SECONDS;
        static const long YEAR_IN_SECONDS;
        Datetime(long long timestamp);
        void setTimestamp(long long timestamp);
        void addSeconds(long long seconds);
        void addMonths(int months);
        void addYears(int years);
        unsigned char getMinutes();
        unsigned char getHours();
        unsigned char getDay();
        unsigned char getMonth();
        int getYear();
        void formatLeadingZero(char *buffer, int value, int length);
        void formatShortMonth(char *buffer, int month);
        long long getTimestamp();
    private:
        unsigned long long timestamp;
        void subtractMonths(int months);
        void subtractYears(int years);
        unsigned long getYearInSeconds(int years);
        void setYearAndReminder(int *year, long *reminder);
};

#endif