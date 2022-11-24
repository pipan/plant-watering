#include <Arduino.h>
#include <IndexView.h>
#include <EmptyView.h>
#include <RotaryEncoder.h>
#include <PushButton.h>
#include <ViewController.h>
#include <U8g2lib.h>
#include <Datetime.h>
#include <Wire.h>
#include <Rtc.h>
#include <LowPower.h>

#define DEBUG 1

#define STATE_RUNNING 0
#define STATE_SLEEP 1

#define SLEEP_LIMIT 15000
#define LOOP_PREFERED_TIME 30

U8G2_SSD1306_128X64_NONAME_2_4W_SW_SPI oled(U8G2_R0, 13, 12, 10, 11, 9);
RotaryEncoder encoder(2, 3);
PushButton encoderButton(4, LOW);
ViewController viewController;
unsigned long lastTick = 0;
char timerTotal = 0;
volatile int idleFor = 0;
volatile char state = STATE_RUNNING;

void wakeUp() {
  Serial.println("WAKE UP");
  state = STATE_RUNNING;
  idleFor = 0;
}

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
  wakeUp();
}

void enterSleepMode() {
  if (state != STATE_SLEEP) {
    state = STATE_SLEEP;
    viewController.home();
    viewController.pushHistory(new EmptyView(&oled, &viewController));
  }
  Wire.end();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  // GOOD NIGHT ARDUINO - arduino is sleeping
  Wire.begin();
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
  
  viewController.pushHistory(new IndexView(&oled, &viewController));

  Rtc::begin();
  Rtc::start();
}

void loop() {
  unsigned long loopStart = millis();
  unsigned long tick = Datetime::msDiff(lastTick, loopStart);
  lastTick = loopStart;

  if (idleFor > SLEEP_LIMIT) {
    enterSleepMode();
  } else {
    idleFor += tick;
    encoder.unqueue();
    encoderButton.read();
    viewController.onTick(tick);
  }
  unsigned long loopDuration = Datetime::msDiff(loopStart, millis());
  if (loopDuration < LOOP_PREFERED_TIME) {
    delay(LOOP_PREFERED_TIME - loopDuration);
  }
  #ifdef DEBUG
  if (loopDuration > LOOP_PREFERED_TIME) {
    Serial.print("Loop duration: ");
    Serial.println(loopDuration);
  }
  #endif
}