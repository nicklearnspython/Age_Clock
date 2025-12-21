#pragma once

#include <Stepper.h>

#include "ClockConstants.h"
#include "Effector.h"

/// Effector implementation that drives a single stepper motor.
class StepperEffector final : public Effector {
public:
  StepperEffector(
    unsigned int stepsPerRevolution,
    int pin1,
    int pin2,
    int pin3,
    int pin4)
    : _stepper(stepsPerRevolution, pin1, pin2, pin3, pin4),
      _day_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _days_per_revolution)),
      _month_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _months_per_revolution)),
      _year_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _years_per_revolution))
  {
    _stepper.setSpeed(stepperSpeedRPM);
  }

  void displayDay(unsigned int day) override {
    driveStepper(day, _current_day_value, _day_steps_per_unit);
  }

  void displayMonth(unsigned int month) override {
    driveStepper(month, _current_month_value, _month_steps_per_unit);
  }

  void displayYear(unsigned int year) override {
    driveStepper(year, _current_year_value, _year_steps_per_unit);
  }

private:
  Stepper _stepper;
  const unsigned int _day_steps_per_unit;
  const unsigned int _month_steps_per_unit;
  const unsigned int _year_steps_per_unit;
  static constexpr unsigned int _days_per_revolution = 31;
  static constexpr unsigned int _months_per_revolution = 12;
  static constexpr unsigned int _years_per_revolution = 18;
  unsigned int _current_day_value = 0;
  unsigned int _current_month_value = 0;
  unsigned int _current_year_value = 0;

  void driveStepper(
    unsigned int newValue,
    unsigned int& currentValue,
    unsigned int stepsPerUnit)
  {
    const long delta = static_cast<long>(newValue) - static_cast<long>(currentValue);
    const long targetSteps = delta * static_cast<long>(stepsPerUnit);

    if (targetSteps == 0) {
      return;
    }

    _stepper.step(static_cast<int>(targetSteps));
    currentValue = newValue;
  }

  static unsigned int calculateStepsPerUnit(
    unsigned int stepsPerRevolution,
    unsigned int unitsPerRevolution) {
    return unitsPerRevolution == 0 ? 0 : stepsPerRevolution / unitsPerRevolution;
  }
};
