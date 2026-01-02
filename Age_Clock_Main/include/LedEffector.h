#pragma once

#include <FastLED.h>
#include "ClockConstants.h"
#include "Effector.h"

/// Effector implementation that uses a single FastLED strip.
class LedEffector final : public Effector {
public:
  LedEffector(
    unsigned int dayLedCount,
    unsigned int monthLedCount,
    unsigned int yearLedCount,
    CRGB::HTMLColorCode activeColor = CRGB::White)
    : _day_led_count(dayLedCount),
      _month_led_count(monthLedCount),
      _year_led_count(yearLedCount),
      _total_led_count(dayLedCount + monthLedCount + yearLedCount),
      _active_color(activeColor)
  {
    _leds = new CRGB[_total_led_count];
    FastLED.addLeds<LED_TYPE, LED_PIN>(_leds, _total_led_count);
    clearStrip();
  }

  ~LedEffector() override {
    delete[] _leds;
  }

  unsigned int totalLedCount() const {
    return _total_led_count;
  }

  void displayDay(unsigned int day) override {
    displayUnit(day, 0, _day_led_count);
  }

  void displayMonth(unsigned int month) override {
    displayUnit(month, _day_led_count, _month_led_count);
  }

  void displayYear(unsigned int year) override {
    const unsigned int offset = _day_led_count + _month_led_count;
    displayUnit(year, offset, _year_led_count);
  }

  void clearStrip() {
    Serial.println("Clearing LED strip");
    fill_solid(_leds, _total_led_count, CRGB::Black);
    FastLED.show();
  }

  void fillStrip() {
    Serial.println("Filling strip with Red");
    fill_solid(_leds, _total_led_count, CRGB::Red);
    FastLED.show();
  }

private:
  const unsigned int _day_led_count;
  const unsigned int _month_led_count;
  const unsigned int _year_led_count;
  const unsigned int _total_led_count;
  const CRGB _active_color;
  CRGB* _leds = nullptr;

  void displayUnit(unsigned int value, unsigned int offset, unsigned int segmentCount) {
    if (segmentCount == 0) {
      return;
    }

    const unsigned int maxValue = segmentCount - 1;
    const unsigned int clampedValue = value > maxValue ? maxValue : value;
    const unsigned int ledsToLight = clampedValue + 1;  // ensure index 0 is always on

    for (unsigned int i = 0; i < segmentCount; ++i) {
      const bool turnOn = i < ledsToLight;
      setLedState(offset + i, turnOn);
    }

    FastLED.show();
  }

  void setLedState(unsigned int index, bool on) {
    if (index >= _total_led_count) {
      return;
    }

    _leds[index] = on ? _active_color : CRGB::Black;
  }
};
