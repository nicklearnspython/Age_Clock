#pragma once

#include <bitswap.h>
#include <chipsets.h>
#include <chsv.h>
#include <cled_controller.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpixel_ledcontroller.h>
#include <cpp_compat.h>
#include <crgb.h>
#include <dither_mode.h>
#include <dmx.h>
#include <eorder.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <FastLED.h>
#include <fastpin.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <fastspi.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <noisegen.h>
#include <pixel_controller.h>
#include <pixel_iterator.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>
#include <rgbw.h>
#include <transpose8x1_noinline.h>

#include "ClockConstants.h"
#include "Effector.h"

#ifdef __INTELLISENSE__
#define FASTLED_HAS_CLOCKLESS 1
#endif
#include <FastLED.h>
#ifndef NEOPIXEL
#define LED_TYPE NEOPIXEL
#endif


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
