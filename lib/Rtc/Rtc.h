#ifndef Rtc_h
#define Rtc_h

class Rtc {
    public:
        static const char DS1307_ADDRESS;
        Rtc();
        void begin();
        void start();
        void stop();
        bool isRunning();
        char getSeconds();
        char getMinutes();
        char getHours();
        void setHours(char hours);
        void setMinutes(char minutes);
        void setSeconds(char seconds);
    private:
        char read(char address);
        void write(char address, char value);
};

#endif
