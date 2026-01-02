/*
 * Nick Garrett
 * The Age Clock
 */

#include <Arduino.h>

#include "include/LedEffector.h"

LedEffector * led_effector;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Built in LED Setup
  setLedHigh();
  Serial.begin(9600);
  Serial.println("Setup Complete. Starting loop.");
  led_effector = new LedEffector(dayLedCount, monthLedCount, yearLedCount);
  FastLED.setBrightness(128);
  FastLED.clearData();
}

void loop() {
  //blink();
  count();
}

void setLedHigh(){
  Serial.println("Setting led high.");
  led_effector->fillStrip();
}

void setLedLow(){
  Serial.println("Setting led low.");
  led_effector->clearStrip();
}

void blink(){
  static unsigned long lastToggle = 0;
  static bool isHigh = false;
  unsigned long now = millis();

  if (now - lastToggle >= 500) {
    lastToggle = now;
    if (isHigh) {
      setLedLow();
    } else {
      setLedHigh();
    }
    isHigh = !isHigh;
  }
}

void count(){
  static unsigned long lastIncrement = 0;
  unsigned long now = millis();
  static int day_incr = 1;

  if (now - lastIncrement >= 500) {
    lastIncrement = now;
    if (day_incr >= daysMax)
    {
      Serial.println("Resetting LED count");
      day_incr = 1;
    }
    else {
      Serial.println("Incrementing LED.");
      ++day_incr;
    }

    led_effector->displayDay(day_incr);
    led_effector->displayMonth(3);
  }
}

//void testDayMonthLeds() {
//  unsigned int currentDay = daysMin;
//  unsigned int currentMonth = monthsMin;
//
//  while (true) {
//    led_effector.displayDay(currentDay);
//    led_effector.displayMonth(currentMonth);
//
//    currentDay++;
//    if (currentDay > static_cast<unsigned int>(daysMax)) {
//      currentDay = daysMin;
//      currentMonth++;
//      if (currentMonth > static_cast<unsigned int>(monthsMax)) {
//        currentMonth = monthsMin;
//      }
//    }
//
//    delay(500);
//  }
//}
