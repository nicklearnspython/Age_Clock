/*
 * Nick Garrett
 * The Age Clock
 */

#include <Arduino.h>

#include "include/LedEffector.h"

LedEffector * led_effector;

void setup() {
  // Starting Serial
  Serial.begin(9600);
  Serial.println("Welcome to the Age Clock. Starting Setup.");

  // Setup the LED Effector
  led_effector = new LedEffector();
  FastLED.setBrightness(32);
  FastLED.clearData();

  // Setup Stepper
  pinMode(stepperHomingSwitchPin, INPUT_PULLUP);

  // Setup Complete
  Serial.println("Setup Complete. Starting loop.");
}

void loop() {
  //blink();
  count();
  //testStepperButton();
  //testStepperButtonWithLeds();
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
  static int day_count = 0;
  static int month_count = 0;

  if (now - lastIncrement >= 500) {
    lastIncrement = now;

    // Increment the day
    ++day_count;

    if (day_count > daysMax)
    {
      Serial.println("Incrementing Month LED count");
      ++month_count;
      day_count = 0;
      led_effector->clearStrip();
    }

    if (month_count > monthsMax)
    {
      Serial.println("Resetting All LED counts");
      month_count = 0;
      day_count = 0;
      led_effector->clearStrip();
    }

    Serial.print("Month count: ");
    Serial.print(month_count);
    Serial.print(", Day count: ");
    Serial.println(day_count);

    led_effector->displayDay(day_count);
    led_effector->displayMonth(month_count);
  }
}

bool isStepperSwitchPressed()
{
  // Based on Debounce Button Example
  // Setup static variables
  static const bool pressedState = LOW;
  static bool buttonState = digitalRead(stepperHomingSwitchPin);
  static bool lastButtonState = HIGH;             // Pullup up so default is high
  static unsigned long lastDebounceTime = 0;      // ms
  static const unsigned long debounceDelay = 50;  // ms

  // read the current homing switch state
  int reading = digitalRead(stepperHomingSwitchPin);

  // If the switch changed regardless of how, reset the debouncing timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      if(buttonState == pressedState)
      {
        lastButtonState = reading;
        return true;
      }
    }
  }

  // Save the current reading
  lastButtonState = reading;
  return false;
}

void testStepperButton(){
  if(isStepperSwitchPressed())
  {
    // For testing purposes, print the change of state
    Serial.println("Stepper button was pressed!");
  }
}

void testStepperButtonWithLeds()
{
  static int day_count = 0;
  static int month_count = 0;
  led_effector->displayDay(day_count);
  led_effector->displayMonth(month_count);

  if(isStepperSwitchPressed())
  {
    ++day_count;
    if (day_count > daysMax)
    {
      ++month_count;
      day_count = 0;
      led_effector->clearStrip();
    }
    Serial.print("The month count is now: ");
    Serial.println(month_count);
    Serial.print("The day count is now: ");
    Serial.println(day_count);
    led_effector->displayDay(day_count);
    led_effector->displayMonth(month_count);
  }
}
