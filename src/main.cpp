#include <Arduino.h>
#include <IndexView.h>
#include <RotaryEncoder.h>
#include <PushButton.h>
#include <ViewController.h>
#include <U8g2lib.h>
#include <Time.h>

U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI oled(U8G2_R0, 13, 12, 10, 11, 9);
RotaryEncoder encoder(2, 3);
PushButton encoderButton(4, LOW);
ViewController *viewController = NULL;
unsigned long lastTick = 0;

void encoderChange(bool clockwise) {
  viewController->onInput(clockwise);
}

void buttonClick() {
  viewController->onClick();
}

void buttonHold() {
  viewController->onHold();
}

void interupt() {
  encoder.queue();
}

void setup() {
  Serial.begin(115200);
  oled.begin();
  encoder.begin();
  encoderButton.begin();

  encoder.onChange(encoderChange);
  encoderButton.onHold(buttonHold, 500);
  encoderButton.onClick(buttonClick);
  attachInterrupt(digitalPinToInterrupt(2), interupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), interupt, CHANGE);
  
  viewController = new ViewController();
  viewController->pushHistory(new IndexView(&oled, viewController));
}

void loop() {
  unsigned long loopStart = millis();
  encoder.unqueue();
  encoderButton.read();
  unsigned long ms = millis();
  viewController->onTick(Time::msDiff(lastTick, ms));
  lastTick = ms;
  unsigned long loopDuration = Time::msDiff(loopStart, millis());
  if (loopDuration < 30) {
    delay(max(0, 30 - loopDuration));
  } else {
    Serial.print("Loop duration: ");
    Serial.println(loopDuration);
  }
}