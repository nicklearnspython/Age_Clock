/// @file Date.h
/// @author Nick Garrett (nicklearnspython@gmail.com)
/// @brief Simple immutable calendar date type (year, month, day).

#pragma once

#include "ClockAge.h"
#include "ClockUtils.h"

/// @class Date
/// @brief Lightweight value type representing a calendar date.
///
/// Holds year, month, and day components. Does not perform validation
/// internally; construction assumes values provided are already valid or
/// come from a validated source (e.g., `DateFactory`).
class Date
{
  public:
    /// @brief Construct from year, month, and day components.
    /// @param year Four-digit year (e.g., 2024).
    /// @param month Month in [1, 12].
    /// @param day Day in [1, 31] (depending on month/year).
    explicit Date(uint16_t year, uint8_t month, uint8_t day);

    /// @brief Construct from year, month, and day components.
    /// @param year Four-digit year (e.g., 2024).
    /// @param day_of_year Day of the year [1, 365 or 365] (depending on leap year).
    explicit Date(uint16_t year, uint16_t day_of_year);

    /// @brief Copy constructor.
    /// @param other Source instance to copy.
    Date(const Date & other);

    /// @brief Returns the year component.
    uint16_t getYear() const {return year;}

    /// @brief Returns the month component.
    uint8_t getMonth() const {return month;}

    /// @brief Returns the day component.
    uint8_t getDay() const {return day;}

    /// @brief Returns the day of the year
    uint16_t getDayOfYear() const {return day_of_year;}

    bool isValid() const {return is_valid;}

    /// @brief Subtraction of two dates is an age
    /// @param other date
    /// @return Age
    Age operator-(const Date & other) const;

    /// @brief Equality comparison on year, month, and day.
    /// @param other Date to compare against.
    /// @return true if all components are equal.
    bool operator==(const Date & other) const;

    /// @brief Inequality comparison on year, month, and day.
    /// @param other Date to compare against.
    /// @return true if any component differs.
    bool operator!=(const Date & other) const;

    /// @brief Strict-weak ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is earlier than other.
    bool operator<(const Date & other) const;

    /// @brief Less-than-or-equal ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is earlier than or equal to other.
    bool operator<=(const Date & other) const;

    /// @brief Greater-than ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is later than other.
    bool operator>(const Date & other) const;

    /// @brief Greater-than-or-equal ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is later than or equal to other.
    bool operator>=(const Date & other) const;

  private:
    uint16_t year;        ///< Year value
    uint8_t month;        ///< Month value [1-12]
    uint8_t day;          ///< Day value [1-31]
    uint16_t day_of_year; ///< The number of days since the start of the year
    bool is_valid;        ///< Validity check on construction
};

Date::Date(uint16_t year, uint8_t month, uint8_t day)
: year(year),
  month(month),
  day(day)
{
  if (
    year < YEARS_MIN || year > YEARS_MAX ||
    month < MONTHS_MIN|| month > MONTHS_MAX ||
    day < DAYS_MIN || day > DAYS_MAX
  )
  {
    is_valid = false;
  }
  else
  {
    day_of_year = daysSinceYearStart(year, month, day);
    is_valid = true;
  }
}

Date::Date(uint16_t year, uint16_t day_of_year)
: year(year),
  day_of_year(day_of_year)
{
  convertDaysOfYearToCalendarMonthDays(year, day_of_year, month, day);
}

Date::Date(const Date & other)
: year(other.year),
  month(other.month),
  day(other.day),
  day_of_year(other.day_of_year)
{}

Age Date::operator-(const Date & birthday) const
{
  int age_day;
  int age_month;
  int age_year;

  int monthModifier = 0;
  int yearModifier = 0;

  int day_diff = day - birthday.day;
  if (day_diff >= 0)
  {
    age_day = day_diff;
  }
  else
  {
    age_day = previousMonthDayCount(year, month) + day_diff;
    monthModifier += 1;

    if (age_day < 0)
    {
      age_day = twoMonthsAgoDayCount(year, month) + age_day;
      monthModifier += 1;
    }
  }

  int month_diff = month - birthday.month - monthModifier;
  if (month_diff >= 0)
  {
    age_month = month_diff;
  }
  else
  {
    age_month = 12 + month_diff;
    yearModifier += 1;
  }

  age_year = year - birthday.year - yearModifier;

  return Age(age_year, age_month, age_day);
}

bool Date::operator==(const Date & other) const
{
  return year == other.year &&
         month == other.month &&
         day == other.day;
}

bool Date::operator!=(const Date & other) const
{
  return !(*this == other);
}

bool Date::operator<(const Date & other) const
{
  if (year != other.year) {return year < other.year;}
  if (month != other.month) {return month < other.month;}
  return day < other.day;
}

bool Date::operator<=(const Date & other) const
{
  return *this < other || *this == other;
}

bool Date::operator>(const Date & other) const
{
  return !(*this <= other);
}

bool Date::operator>=(const Date & other) const
{
  return !(*this < other);
}
