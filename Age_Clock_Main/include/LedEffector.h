/// @file LedEffector.h
/// @brief LED strip effector object to display days, months, or years.

#pragma once

#include <FastLED.h>
#include "ClockConstants.h"
#include "Effector.h"

/// Effector implementation that uses a single FastLED strip.
class LedEffector final : public Effector {
public:
  LedEffector(CRGB::HTMLColorCode activeColor = CRGB::White)
    : _total_led_count(DAY_LED_COUNT + MONTH_LED_COUNT + YEAR_LED_COUNT + 1), // Led 0 is not displaying so add an additional led
      _active_color(activeColor)
  {
    // The month and day led strands are a single continuous strand
    _all_leds = new CRGB[_total_led_count];
    _first_month_led = _all_leds + 1;                  // the month strand is first
    _first_day_led = _first_month_led + MONTH_LED_COUNT; // the day strand is second
    _first_year_led = _first_day_led + DAY_LED_COUNT;    // the hypothetical year strand is last

    FastLED.addLeds<LED_TYPE, LED_PIN>(_all_leds, _total_led_count);
    clearStrip();
  }

  ~LedEffector() override {
    delete[] _all_leds;
  }

  unsigned int totalLedCount() const {
    return _total_led_count;
  }

  void displayDay(unsigned int day) override {
    fill_solid(_first_day_led, LEDS_PER_DAY * (day + 1), _active_color);
    FastLED.show();
  }

  void displayMonth(unsigned int month) override {
    fill_solid(_first_month_led, LEDS_PER_MONTH * (month + 1), _active_color);
    FastLED.show();
  }

  void displayYear(unsigned int year) override {
    fill_solid(_first_year_led, LEDS_PER_MONTH * (year + 1), _active_color);
    FastLED.show();
  }

  void clearStrip() {
    Serial.println("Clearing LED strip");
    fill_solid(_all_leds, _total_led_count, CRGB::Black);
    FastLED.show();
  }

  void fillStrip() {
    Serial.println("Filling strip with Red");
    fill_solid(_all_leds, _total_led_count, CRGB::Red);
    FastLED.show();
  }

private:
  const unsigned int _total_led_count;
  const CRGB _active_color;
  CRGB* _all_leds = nullptr;
  CRGB* _first_month_led = nullptr;
  CRGB* _first_day_led = nullptr;
  CRGB* _first_year_led = nullptr;
};
