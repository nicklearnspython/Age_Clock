#pragma once

/// Constants

bool isAutoplayEnabled = true;

static const uint8_t RXPin = 4, TXPin = 3;
static const uint8_t dummy_RXPin = 13, dummy_TXPin = 12;
static const uint32_t GPSBaud = 9600;

//C
static const int birthDay = 30;
static const int birthMonth = 12;
static const int birthYear = 2016;

//P
//static const int birthDay = 10;
//static const int birthMonth = 11;
//static const int birthYear = 2018;

//J
//static const int birthDay = 17;
//static const int birthMonth = 6;
//static const int birthYear = 2021;

static const String dayServoTitle = "Day";
static const String monthServoTitle = "Month";
static const String yearServoTitle = "Year";

static const int dayServoPin = 11;
static const int monthServoPin = 10;
static const int yearServoPin = 9;

static const int stepperStepsPerRevolution = 2048;
static const int stepperPin1 = 8;
static const int stepperPin2 = 9;
static const int stepperPin3 = 10;
static const int stepperPin4 = 11;
static const int stepperSpeedRPM = 10;

static const int daysMin = 0;
static const int daysMax = 30;
static const int monthsMin = 0;
static const int monthsMax = 11;
static const int yearsMin = 0;
static const int yearsMax = 17;

static const int dayServoAngleMin = 0;
static const int dayServoAngleMax = 180;
static const int monthServoAngleMin = 0;
static const int monthServoAngleMax = 180;
static const int yearServoAngleMin = 0;
static const int yearServoAngleMax = 180;

static const uint8_t dayAngles[] = {0, 5, 9, 13, 20, 26, 31, 36, 41, 47, 51, 56, 62, 66, 72, 78, 83, 89, 94, 100, 106, 113, 119, 125, 131, 138, 144, 152, 162, 166, 175};
static const uint8_t monthAngles[] = {0, 15, 30, 44, 58, 72, 85, 100, 117, 133, 150, 170};
static const uint8_t yearAngles[] = {0, 8, 18, 27, 37, 47, 57, 68, 80, 92, 104, 117, 130, 141, 154, 166, 179};

// Offset hours from gps time (UTC)
static const int CST_offset = -6;  // Central Standard Time (USA)
static const int CDT_offset = -5;  // Central Daylight Time (USA)
static const int PST_offset = -8;  // Pacific Standard Time (USA)
static const int PDT_offset = -7;  // Pacific Daylight Time (USA)
