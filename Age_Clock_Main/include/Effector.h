/// @file Effector.h
/// @brief Abstract interface for age display effectors.

#pragma once

/// Abstract interface for anything that can display age information.
class Effector {
public:
  virtual ~Effector() = default;

  virtual void displayDay(unsigned int day) = 0;
  virtual void displayMonth(unsigned int month) = 0;
  virtual void displayYear(unsigned int year) = 0;

protected:
  Effector() = default;

private:
  Effector(const Effector&) = delete;
  Effector& operator=(const Effector&) = delete;
};
