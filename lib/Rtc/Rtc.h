#ifndef Rtc_h
#define Rtc_h

class Rtc {
    public:
        static const char DS1307_ADDRESS;
        static void begin();
        static void start();
        static void stop();
        static bool isRunning();
        static char getSeconds();
        static char getMinutes();
        static char getHours();
        static char getDate();
        static char getMonth();
        static int getYear();
        static char getYearChar();
        static void setHours(char hours);
        static void setMinutes(char minutes);
        static void setSeconds(char seconds);
        static void setDate(char date);
        static void setMonth(char month);
        static void setYear(int year);
    private:
        static char read(char address);
        static char readDecimal(char address);
        static void write(char address, char value);
        static void writeDecimal(char address, char value);
};

#endif
