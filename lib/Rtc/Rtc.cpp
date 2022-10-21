#include "Rtc.h"
#include <Wire.h>

const char Rtc::DS1307_ADDRESS = 0b1101000;

Rtc::Rtc() { }

void Rtc::begin() {
    Wire.begin();
}

void Rtc::start() {
    char seconds = this->getSeconds();
    this->write(0x00, seconds & 0b01111111);
}

void Rtc::stop() {
    char seconds = this->getSeconds();
    this->write(0x00, seconds | 0b10000000);
}

bool Rtc::isRunning() {
    return !(this->read(0x00) & 0b10000000);
}

char Rtc::getSeconds() {
    char data = this->read(0x00) & 0b01111111;
    return (data >> 4) * 10 + (data & 0x0f);
}

char Rtc::getMinutes() {
    char data = this->read(0x01) & 0b01111111;
    return (data >> 4) * 10 + (data & 0x0f);
}

char Rtc::getHours() {
    char data = this->read(0x02) & 0b00111111;
    return (data >> 4) * 10 + (data & 0x0f);
}

void Rtc::setHours(char hours) {
    char data = ((hours / 10) << 4) + (hours % 10);
    this->write(0x02, data);
}

void Rtc::setMinutes(char minutes) {
    char data = ((minutes / 10) << 4) + (minutes % 10);
    this->write(0x01, data);
}

void Rtc::setSeconds(char seconds) {
    bool running = this->isRunning();
    char data = ((seconds / 10) << 4) + (seconds % 10);
    if (!running) {
        data |= 0b10000000;
    }
    this->write(0x02, data);
}

char Rtc::read(char address) {
    Wire.beginTransmission(Rtc::DS1307_ADDRESS);
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(Rtc::DS1307_ADDRESS, 1);
    return Wire.read();
}

void Rtc::write(char address, char value) {
    Wire.beginTransmission(Rtc::DS1307_ADDRESS);
    Wire.write(address);
    Wire.write(value);
    Wire.endTransmission();
}