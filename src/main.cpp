#include <Arduino.h>
#include <IndexView.h>
#include <EmptyView.h>
#include <RotaryEncoder.h>
#include <PushButton.h>
#include <ViewController.h>
#include <U8g2lib.h>
#include <Datetime.h>
#include <Timer.h>
#include <Wire.h>
#include <Rtc.h>
#include <LowPower.h>

U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI oled(U8G2_R0, 13, 12, 10, 11, 9);
RotaryEncoder encoder(2, 3);
PushButton encoderButton(4, LOW);
ViewController viewController;
Timer timer;
unsigned long lastTick = 0;
const char TIMER_LENGH = 4;
char timerTotal = 0;
int idleFor = 0;

void encoderChange(bool clockwise) {
  viewController.onInput(clockwise);
  idleFor = 0;
}

void buttonClick() {
  viewController.onClick();
  idleFor = 0;
}

void buttonHold() {
  viewController.onHold();
  idleFor = 0;
}

void interupt() {
  encoder.queue();
}

ISR(TIMER1_COMPA_vect){
  timerTotal += TIMER_LENGH;
  if (timerTotal < 60) {
    return;
  }
  timerTotal = 0;
}

void enterSleepMode() {
  Wire.end();
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  // ADCSRA = 0;
  // set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  // noInterrupts ();
  // sleep_enable();
  // MCUCR = bit (BODS) | bit (BODSE);
  // MCUCR = bit (BODS);
  // sleep_bod_disable();
  // interrupts();

  // sleep_mode(); 
  // GOOD NIGHT ARDUINO - arduino is sleeping
  Wire.begin();
}

void setup() {
  Serial.begin(115200);
  oled.begin();
  encoder.begin();
  encoderButton.begin();

  timer.beginHz(TIMER_LENGH);

  encoder.onChange(encoderChange);
  encoderButton.onHold(buttonHold, 500);
  encoderButton.onClick(buttonClick);
  attachInterrupt(digitalPinToInterrupt(2), interupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), interupt, CHANGE);
  
  viewController.pushHistory(new IndexView(&oled, &viewController));

  Rtc::begin();
  Rtc::start();
}

void loop() {
  unsigned long loopStart = millis();
  encoder.unqueue();
  encoderButton.read();
  unsigned long ms = millis();
  viewController.onTick(Datetime::msDiff(lastTick, ms));
  lastTick = ms;
  unsigned long loopDuration = Datetime::msDiff(loopStart, millis());
  if (loopDuration < 30) {
    delay(max(0, 30 - loopDuration));
  } else {
    Serial.print("Loop duration: ");
    Serial.println(loopDuration);
  }
  idleFor += max(30, loopDuration);
  if (idleFor > 10000) {
    idleFor = 0;
    viewController.home();
    viewController.pushHistory(new EmptyView(&oled, &viewController));
    enterSleepMode();
    lastTick = millis();
  }
}