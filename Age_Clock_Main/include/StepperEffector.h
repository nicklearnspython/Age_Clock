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
      _steps_per_revolution(stepsPerRevolution),
      _day_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _days_per_revolution)),
      _month_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _months_per_revolution)),
      _year_steps_per_unit(calculateStepsPerUnit(stepsPerRevolution, _years_per_revolution))
  {
    _stepper.setSpeed(STEPPER_RPM);
  }

  void begin(int homePin, long zeroOffsetSteps) {
    _home_pin = homePin;
    _home_offset_steps = zeroOffsetSteps;
    _is_homed = false;

    if (_home_pin >= 0) {
      pinMode(static_cast<uint8_t>(_home_pin), INPUT_PULLUP);
    }
  }

  bool home() {
    if (_home_pin < 0) {
      Serial.println("ERROR: Homing pin value must be nonnegative.");
      return false;
    }

    const long max_search_steps = static_cast<long>(_steps_per_revolution) * static_cast<long>(_max_home_search_revolutions);
    long steps_travelled = 0;

    while (digitalRead(static_cast<uint8_t>(_home_pin)) == HIGH && steps_travelled > -max_search_steps) {
      _stepper.step(-static_cast<int>(_homing_step_size));
      steps_travelled -= static_cast<long>(_homing_step_size);
    }

    if (digitalRead(static_cast<uint8_t>(_home_pin)) == HIGH) {
      return false;
    }

    delay(_homing_settle_delay_ms);

    if (_home_offset_steps != 0) {
      _stepper.step(static_cast<int>(_home_offset_steps));
    }

    _current_day_value = 0;
    _current_month_value = 0;
    _current_year_value = 0;
    _is_homed = true;
    return true;
  }

  bool isHomed() const {
    return _home_pin < 0 || _is_homed;
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
  const unsigned int _steps_per_revolution;
  const unsigned int _day_steps_per_unit;
  const unsigned int _month_steps_per_unit;
  const unsigned int _year_steps_per_unit;
  static constexpr unsigned int _days_per_revolution = 31;
  static constexpr unsigned int _months_per_revolution = 12;
  static constexpr unsigned int _years_per_revolution = 18;
  unsigned int _current_day_value = 0;
  unsigned int _current_month_value = 0;
  unsigned int _current_year_value = 0;
  int _home_pin = -1;
  long _home_offset_steps = 0;
  bool _is_homed = false;
  static constexpr unsigned int _homing_step_size = 4;
  static constexpr unsigned int _max_home_search_revolutions = 4;
  static constexpr unsigned int _homing_settle_delay_ms = 10;

  void driveStepper(
    unsigned int newValue,
    unsigned int& currentValue,
    unsigned int stepsPerUnit)
  {
    if (!isHomed()) {
      return;
    }

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

// Stepper motor getting hot solution
// https://forum.arduino.cc/t/stepper-28byj-48-is-hot-at-150f-is-this-normal/1205109/11