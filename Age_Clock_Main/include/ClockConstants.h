/// @file ClockConstants.h
/// @brief Project-wide hardware and configuration constants.

#pragma once

#include <FastLED.h>

/// ---------------------------------------------------------------------------
/// ------------------------------- CONSTANTS ---------------------------------
/// ---------------------------------------------------------------------------

bool isAutoplayEnabled = true;

/// ----------------------------- AGE CONSTANTS -------------------------------
static const int DAYS_MIN = 0;
static const int DAYS_MAX = 30;
static const int MONTHS_MIN = 0;
static const int MONTHS_MAX = 11;
static const int YEARS_MIN = 0;
static const int YEARS_MAX = 17;

//C
static const int BIRTH_DAY = 30;
static const int BIRTH_MONTH = 12;
static const int BIRTH_YEAR = 2016;

//P
//static const int BIRTH_DAY = 10;
//static const int BIRTH_MONTH = 11;
//static const int BIRTH_YEAR = 2018;

//J
//static const int BIRTH_DAY = 17;
//static const int BIRTH_MONTH = 6;
//static const int BIRTH_YEAR = 2021;

/// --------------------------- STEPPER CONSTANTS -----------------------------
static constexpr int STEPPER_PIN_1 = 8;
static constexpr int STEPPER_PIN_2 = 9;
static constexpr int STEPPER_PIN_3 = 10;
static constexpr int STEPPER_PIN_4 = 11;

static constexpr int STEPPER_STEPS_PER_REV = 2048;
static constexpr int STEPPER_RPM = 10;

static constexpr int STEPPER_HOMING_PIN = 5;
static constexpr long HOMING_OFFSET_STEPS = 0;

/// ----------------------------- LED CONSTANTS -------------------------------
static constexpr int LED_PIN = 6;
#define LED_TYPE NEOPIXEL         // Previously used WS2812B
#define COLOR_ORDER GRB

static constexpr unsigned int LEDS_PER_DAY = 1;
static constexpr unsigned int LEDS_PER_MONTH = 2;
static constexpr unsigned int LEDS_PER_YEAR = 0;
static constexpr unsigned int DAY_LED_COUNT = (DAYS_MAX + 1) * LEDS_PER_DAY;
static constexpr unsigned int MONTH_LED_COUNT = (MONTHS_MAX + 1) * LEDS_PER_MONTH;
static constexpr unsigned int YEAR_LED_COUNT = (YEARS_MAX + 1) * LEDS_PER_YEAR;

/// ---------------------------- SERVO CONSTANTS ------------------------------
static const int dayServoPin = 11;
static const int monthServoPin = 10;
static const int yearServoPin = 9;

static const int dayServoAngleMin = 0;
static const int dayServoAngleMax = 180;
static const int monthServoAngleMin = 0;
static const int monthServoAngleMax = 180;
static const int yearServoAngleMin = 0;
static const int yearServoAngleMax = 180;

static const uint8_t dayAngles[] = {0, 5, 9, 13, 20, 26, 31, 36, 41, 47, 51, 56, 62, 66, 72, 78, 83, 89, 94, 100, 106, 113, 119, 125, 131, 138, 144, 152, 162, 166, 175};
static const uint8_t monthAngles[] = {0, 15, 30, 44, 58, 72, 85, 100, 117, 133, 150, 170};
static const uint8_t yearAngles[] = {0, 8, 18, 27, 37, 47, 57, 68, 80, 92, 104, 117, 130, 141, 154, 166, 179};

/// ----------------------------- GPS CONSTANTS -------------------------------
static const int GPS_RX_PIN = 4;
static const int GPS_TX_PIN = 3;
static const uint8_t dummy_RXPin = 13, dummy_TXPin = 12;
static const uint32_t GPS_BAUD = 9600;

// Offset hours from gps time (UTC)
static const int CST_offset = -6;  // Central Standard Time (USA)
static const int CDT_offset = -5;  // Central Daylight Time (USA)
static const int PST_offset = -8;  // Pacific Standard Time (USA)
static const int PDT_offset = -7;  // Pacific Daylight Time (USA)
