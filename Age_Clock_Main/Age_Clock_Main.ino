/*
 * Nick Garrett
 * The Age Clock
 */

#include <Arduino.h>

#include "include/GpsTimeSensor.h"
#include "include/LedEffector.h"
#include "include/StepperEffector.h"
#include "include/ClockAge.h"
#include "include/ClockDate.h"

GpsTimeSensor * gps_time_sensor;
LedEffector * led_effector;
StepperEffector * year_effector;

void setup() {
  // Starting Serial
  Serial.begin(115200);
  Serial.println("Welcome to the Age Clock. Starting Setup.");

  // Setup the GPS Time Sensor
  gps_time_sensor = new GpsTimeSensor;

  // Setup the LED Effector
  led_effector = new LedEffector();
  FastLED.setBrightness(32);
  FastLED.clearData();

  // Setup Stepper
  year_effector = new StepperEffector(
    STEPPER_STEPS_PER_REV,
    STEPPER_PIN_1,
    STEPPER_PIN_2,
    STEPPER_PIN_3,
    STEPPER_PIN_4);
  pinMode(STEPPER_HOMING_PIN, INPUT_PULLUP);

  Serial.println("Initializing the Stepper Motor");
  year_effector->begin(STEPPER_HOMING_PIN, HOMING_OFFSET_STEPS);
  if (!year_effector->home()) {
    Serial.println("Warning: Year stepper homing failed.");
  }
  else {
    Serial.println("Stepper Motor Homed.");
  }

  // Setup Complete
  Serial.println("Setup Complete. Starting loop.");
}

void loop() {
  //blink();
  //count();
  //testStepperButton();
  //testStepperButtonWithLeds();
  //testStepper();
  //testGpsComms();
  //readRawGps();
  testAgeCalculation();
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
  static int year_count = 0;

  if (now - lastIncrement >= 10) {
    lastIncrement = now;

    // Increment the day
    ++day_count;

    if (day_count > DAYS_MAX)
    {
      Serial.println("Max Days. Incrementing Month LED count");
      ++month_count;
      day_count = 0;
      led_effector->clearStrip();
    }

    if (month_count > MONTHS_MAX)
    {
      Serial.println("Max Months. Resetting All LED counts");
      ++year_count;
      month_count = 0;
      day_count = 0;
      led_effector->clearStrip();
    }

    if (year_count > YEARS_MAX)
    {
      Serial.println("Max Years. Resetting all effectors.");
      year_count = 0;
      month_count = 0;
      day_count = 0;
      led_effector->clearStrip();
    }

    Serial.print("Year count: ");
    Serial.print(year_count);
    Serial.print(", Month count: ");
    Serial.print(month_count);
    Serial.print(", Day count: ");
    Serial.println(day_count);

    led_effector->displayDay(day_count);
    led_effector->displayMonth(month_count);
    year_effector->displayYear(year_count);
  }
}

bool isStepperSwitchPressed()
{
  // Based on Debounce Button Example
  // Setup static variables
  static const bool pressedState = LOW;
  static bool buttonState = digitalRead(STEPPER_HOMING_PIN);
  static bool lastButtonState = HIGH;             // Pullup up so default is high
  static unsigned long lastDebounceTime = 0;      // ms
  static const unsigned long debounceDelay = 50;  // ms

  // read the current homing switch state
  int reading = digitalRead(STEPPER_HOMING_PIN);

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
    if (day_count > DAYS_MAX)
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

void testStepper(){
  static int year = 0;
  year_effector->displayYear(year);

  if(isStepperSwitchPressed())
  {
    ++year;
    Serial.print("Incrementing year: ");
    Serial.println(year);
    year_effector->displayYear(year);
  }
}

void testGpsComms()
{
  static int day_count = 0;
  static int month_count = 0;
  led_effector->displayDay(day_count);
  led_effector->displayMonth(month_count);

  if(gps_time_sensor->listen())
  {
    ++day_count;
    if (day_count > DAYS_MAX)
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

void readRawGps()
{
  gps_time_sensor->listen();
}

void testAgeCalculation()
{
  static unsigned long lastIncrement = 0;
  unsigned long now = millis();
  static int day_of_year = 0;

  if (now - lastIncrement >= 500) { // 0.5 seconds
    lastIncrement = now;

    ++day_of_year; // Increment the day

    Date birth(2016, 12, 30);
    Date today(2026, day_of_year);

    Age age = today - birth;

    Serial.print("Birthday: ");
    Serial.print(birth.getYear());
    Serial.print("/");
    Serial.print(birth.getMonth());
    Serial.print("/");
    Serial.print(birth.getDay());
    Serial.print(" | Tomorrow: ");
    Serial.print(today.getYear());
    Serial.print("/");
    Serial.print(today.getMonth());
    Serial.print("/");
    Serial.print(today.getDay());
    Serial.print(" | Age: ");
    Serial.print(age.getYear());
    Serial.print("/");
    Serial.print(age.getMonth());
    Serial.print("/");
    Serial.println(age.getDay());
  }
}
