/// @file Age.h
/// @author Nick Garrett (nicklearnspython@gmail.com)
/// @brief Simple age date type which is a difference between two dates

#pragma once

/// @class Age
class Age
{
  public:
    /// @brief Default Constructor
    Age();

    /// @brief Construct from year, month, and day components.
    /// @param year Four-digit year (e.g., 2024).
    /// @param month Month in [0, 11].
    /// @param day Day in [0, 30] (depending on month/year).
    explicit Age(uint16_t year, uint8_t month, uint8_t day);

    /// @brief Copy constructor.
    /// @param other Source instance to copy.
    Age(const Age & other);

    /// @brief Returns the year component.
    uint16_t getYear() const {return years;}

    /// @brief Returns the month component.
    uint8_t getMonth() const {return months;}

    /// @brief Returns the day component.
    uint8_t getDay() const {return days;}

    /// @brief Equality comparison on year, month, and day.
    /// @param other Date to compare against.
    /// @return true if all components are equal.
    bool operator==(const Age & other) const;

    /// @brief Inequality comparison on year, month, and day.
    /// @param other Date to compare against.
    /// @return true if any component differs.
    bool operator!=(const Age & other) const;

    /// @brief Strict-weak ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is earlier than other.
    bool operator<(const Age & other) const;

    /// @brief Less-than-or-equal ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is earlier than or equal to other.
    bool operator<=(const Age & other) const;

    /// @brief Greater-than ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is later than other.
    bool operator>(const Age & other) const;

    /// @brief Greater-than-or-equal ordering by (year, month, day).
    /// @param other Date to compare against.
    /// @return true if this date is later than or equal to other.
    bool operator>=(const Age & other) const;

    int years; ///< Year value
    int months; ///< Month value [1-12]
    int days;   ///< Day value [1-31]
};

Age::Age()
: years(0),
  months(0),
  days(0)
{}

Age::Age(uint16_t year, uint8_t month, uint8_t day)
: years(year),
  months(month),
  days(day)
{}

Age::Age(const Age & other)
: years(other.years),
  months(other.months),
  days(other.days)
{}

bool Age::operator==(const Age & other) const
{
  return years == other.years &&
         months == other.months &&
         days == other.days;
}

bool Age::operator!=(const Age & other) const
{
  return !(*this == other);
}

bool Age::operator<(const Age & other) const
{
  if (years != other.years) {return years < other.years;}
  if (months != other.months) {return months < other.months;}
  return days < other.days;
}

bool Age::operator<=(const Age & other) const
{
  return *this < other || *this == other;
}

bool Age::operator>(const Age & other) const
{
  return !(*this <= other);
}

bool Age::operator>=(const Age & other) const
{
  return !(*this < other);
}
