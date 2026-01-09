/// @file ClockUtils.h
/// @brief Date-related constants and helper functions.

#pragma once

/// @brief Numeric month value for January.
static constexpr uint8_t JANUARY = 1;
/// @brief Numeric month value for February.
static constexpr uint8_t FEBRUARY = 2;
/// @brief Numeric month value for December.
static constexpr uint8_t DECEMBER = 12;

/// @brief Inclusive lower bound for valid days.
static constexpr uint8_t DAY_LOWER_BOUND = 1;
/// @brief Upper bound for 31-day months.
static constexpr uint8_t DAY_31_UPPER_BOUND = 31;
/// @brief Upper bound for 30-day months.
static constexpr uint8_t DAY_30_UPPER_BOUND = 30;
/// @brief Upper bound for February in leap years.
static constexpr uint8_t DAY_29_UPPER_BOUND = 29;
/// @brief Upper bound for February in non-leap years.
static constexpr uint8_t DAY_28_UPPER_BOUND = 28;

/// @brief Inclusive lower bound for valid number of days in a year
static constexpr uint16_t DAY_OF_YEAR_LOWER_BOUND = 1;
/// @brief Inclusive upper bound for valid number of days in a year
static constexpr uint16_t DAY_OF_YEAR_UPPER_BOUND = 365;
/// @brief Inclusive upper bound for valid number of days in a leap year
static constexpr uint16_t DAY_OF_LEAP_YEAR_UPPER_BOUND = 366;

/// @brief Inclusive lower bound for valid months.
static constexpr uint8_t MONTH_LOWER_BOUND = 1;
/// @brief Inclusive upper bound for valid months.
static constexpr uint8_t MONTH_UPPER_BOUND = 12;

/// @brief Inclusive lower bound for valid years.
static constexpr uint16_t YEAR_LOWER_BOUND = 1;
/// @brief Inclusive upper bound for valid years.
static constexpr uint16_t YEAR_UPPER_BOUND = 4000;

/// @brief Baseline days per month assuming a non-leap February.
/// @note February is set to 28 here; use dayUpperBound(y, m) to obtain the
///       correct bound that accounts for leap years.
//static const std::map<uint8_t, uint8_t> DAYS_IN_MONTH_MAP = {
//  {1, DAY_31_UPPER_BOUND},
//  {2, DAY_28_UPPER_BOUND},
//  {3, DAY_31_UPPER_BOUND},
//  {4, DAY_30_UPPER_BOUND},
//  {5, DAY_31_UPPER_BOUND},
//  {6, DAY_30_UPPER_BOUND},
//  {7, DAY_31_UPPER_BOUND},
//  {8, DAY_31_UPPER_BOUND},
//  {9, DAY_30_UPPER_BOUND},
//  {10, DAY_31_UPPER_BOUND},
//  {11, DAY_30_UPPER_BOUND},
//  {12, DAY_31_UPPER_BOUND},
//};

static uint8_t daysInMonthMap(uint8_t month)
{
  switch (month)
  {
    case 1: return DAY_31_UPPER_BOUND;
    case 2: return DAY_28_UPPER_BOUND;
    case 3: return DAY_31_UPPER_BOUND;
    case 4: return DAY_30_UPPER_BOUND;
    case 5: return DAY_31_UPPER_BOUND;
    case 6: return DAY_30_UPPER_BOUND;
    case 7: return DAY_31_UPPER_BOUND;
    case 8: return DAY_31_UPPER_BOUND;
    case 9: return DAY_30_UPPER_BOUND;
    case 10: return DAY_31_UPPER_BOUND;
    case 11: return DAY_30_UPPER_BOUND;
    case 12: return DAY_31_UPPER_BOUND;
  }

  Serial.println("ERROR: daysInMonthMap out of bounds.");
  return 31;
}

/// @brief Returns whether a year is a Gregorian leap year.
/// @param y Year to evaluate.
/// @return true if divisible by 4 and not by 100, unless divisible by 400.
bool isLeapYear(uint16_t y);

/// @brief Number of days in a month for a given year.
/// @param y Year in [YEAR_LOWER_BOUND, YEAR_UPPER_BOUND].
/// @param m Month in [MONTH_LOWER_BOUND, MONTH_UPPER_BOUND].
/// @return The inclusive upper bound for valid days (28–31).
/// @details February varies with leap years per isLeapYear(y).
uint8_t dayUpperBound(uint16_t y, uint8_t m);

/// @brief Determines the number of days in the previous month.
/// @param y Current year.
/// @param m Current month. Precondition: m in [1, 12].
/// @return uint8_t Days in the previous month.
uint8_t previousMonthDayCount(uint16_t y, uint8_t m);

/// @brief Determines the number of days in the month before last.
/// @param y Current year.
/// @param m Current month. Precondition: m in [1, 12].
/// @return uint8_t The number of days in the month two months ago.
uint8_t twoMonthsAgoDayCount(uint16_t y, uint8_t m);

/// @brief Determines the number of days since the year started
/// @param y Current year
/// @param m Current month.
/// @param d Current day.
/// @return uint16_t
uint16_t daysSinceYearStart(uint16_t y, uint8_t m, uint8_t d);

/// @brief Determines the number of days and months old when given the days since the year started
/// @param y Current year
/// @param doy Current day of the year
/// @param m Current month
/// @param d Current day of the month
/// @return void
void convertDaysOfYearToCalendarMonthDays(uint16_t y, uint16_t doy, uint8_t & m, uint8_t & d);

bool isLeapYear(uint16_t y)
{
  return (y % 4 == 0 && y % 100 != 0) ||
         (y % 400 == 0);
}

uint8_t dayUpperBound(uint16_t y, uint8_t m)
{
  // If a normal month
  if (m != FEBRUARY)
  {
    return daysInMonthMap(m);
  }

  // February on a leap year or not
  return isLeapYear(y) ? DAY_29_UPPER_BOUND : DAY_28_UPPER_BOUND;
}

uint8_t previousMonthDayCount(uint16_t y, uint8_t m)
{
  // If current month is January, we return the days of December.
  // Otherwise, return the month's before
  // The year only matters in February.
  return dayUpperBound(y, m == JANUARY ? DECEMBER : m - 1);
}

uint8_t twoMonthsAgoDayCount(uint16_t y, uint8_t m)
{
  // If current month is January, get the previous month of December.
  // Otherwise, return the month before last
  return previousMonthDayCount(y, m == JANUARY ? DECEMBER : m - 1);
}

uint16_t daysSinceYearStart(uint16_t y, uint8_t m, uint8_t d)
{
  // Init days in the year
  uint16_t doy{0};

  // Start with the month before the current
  --m;

  // Then, add the max number of days of every month prior to the current month
  while (m > 0) {
    doy += dayUpperBound(y, m);
    --m;
  }

  // Finally add the current month's day count
  doy += d;

  return doy;
}

void convertDaysOfYearToCalendarMonthDays(
  uint16_t y,
  uint16_t doy,
  uint8_t & m,
  uint8_t & d)
{
  m = 1;

  while (doy > dayUpperBound(y, m)) {
    doy -= dayUpperBound(y, m);
    m++;
  }

  d = doy;
}
