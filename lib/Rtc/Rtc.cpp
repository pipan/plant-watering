#include "Rtc.h"
#include <Wire.h>
#include <Arduino.h>

const char Rtc::DS1307_ADDRESS = 0b1101000;

void Rtc::begin() {
    Wire.begin();
}

void Rtc::start() {
    char seconds = Rtc::getSeconds();
    Rtc::write(0x00, seconds & 0b01111111);
}

void Rtc::stop() {
    char seconds = Rtc::getSeconds();
    Rtc::write(0x00, seconds | 0b10000000);
}

bool Rtc::isRunning() {
    return !(Rtc::read(0x00) & 0b10000000);
}

char Rtc::getSeconds() {
    char data = Rtc::read(0x00) & 0b01111111;
    return (data >> 4) * 10 + (data & 0x0f);
}

char Rtc::getMinutes() {
    return Rtc::readDecimal(0x01);
}

char Rtc::getHours() {
    char data = Rtc::read(0x02) & 0b00111111;
    return (data >> 4) * 10 + (data & 0x0f);
}

char Rtc::getDate() {
    return Rtc::readDecimal(0x04);
}

char Rtc::getMonth() {
    return Rtc::readDecimal(0x05);
}

char Rtc::getYearChar() {
    return Rtc::readDecimal(0x06);
}

int Rtc::getYear() {
    return Rtc::getYearChar() + 2000;
}

void Rtc::setSeconds(char seconds) {
    bool running = Rtc::isRunning();
    char data = ((seconds / 10) << 4) + (seconds % 10);
    if (!running) {
        data |= 0b10000000;
    }
    Rtc::write(0x02, data);
}

void Rtc::setMinutes(char minutes) {
    Rtc::writeDecimal(0x01, minutes);
}

void Rtc::setHours(char hours) {
    Rtc::writeDecimal(0x02, hours);
}

void Rtc::setDate(char date) {
    Rtc::writeDecimal(0x04, date);
}

void Rtc::setMonth(char month) {
    Rtc::writeDecimal(0x05, month);
}

void Rtc::setYear(int year) {
    Rtc::writeDecimal(0x06, year - 2000);
}

char Rtc::read(char address) {
    Wire.beginTransmission(Rtc::DS1307_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(Rtc::DS1307_ADDRESS, 1);
    return Wire.read();
}

char Rtc::readDecimal(char address) {
    char data = Rtc::read(address);
    return (data >> 4) * 10 + (data & 0x0f);
}

void Rtc::write(char address, char value) {
    Wire.beginTransmission(Rtc::DS1307_ADDRESS);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}

void Rtc::writeDecimal(char address, char value) {
    char data = ((value / 10) << 4) + (value % 10);
    Rtc::write(address, data);
}