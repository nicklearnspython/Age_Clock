/*
 * Nick Garrett
 * The Age Clock
 */

#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#include "AgeCalculator.h"
#include "ClockConstants.h"
#include "ClockServo.h"

ClockServo dayServo(dayServoTitle, dayServoPin, dayServoAngleMin, dayServoAngleMax, daysMin, daysMax);
ClockServo monthServo(monthServoTitle, monthServoPin, monthServoAngleMin, monthServoAngleMax, monthsMin, monthsMax);
ClockServo yearServo(yearServoTitle, yearServoPin, yearServoAngleMin, yearServoAngleMax, yearsMin, yearsMax);

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);                    // The serial connection to the GPS device
SoftwareSerial dummy_ss(dummy_RXPin, dummy_TXPin);  // The serial connection to a dummy serial device. This prevents interrupts to servo control
time_t prevDisplay = 0;

int input = 0;

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  dummy_ss.begin(GPSBaud);

  Serial.println("");
  Serial.println("------- Age Clock Initiate -------");
}


void loop(){
  printMenu();

  if (isAutoplayEnabled){
    input  = 9;
  }
  else {
    while (Serial.available() == 0) {}
    input = Serial.parseInt();
  }

  Serial.print("You inputed the value: ");
  Serial.println(input);

  switch (input) {

    case 1:
      testAgeCalculater();
      break;

    case 2:
      listenForGPSMessages();
      break;

    case 3:
      listenForGPSMessages();
      calculateAge(day(), month(), year(), ageDay, ageMonth, ageYear);
      break;

    case 4:
      dayServo.zero();
      monthServo.zero();
      yearServo.zero();
      break;

    case 5:
      dayServo.center();
      monthServo.center();
      yearServo.center();
      break;

    case 6:
      dayServo.sweep();
      monthServo.sweep();
      yearServo.sweep();
      break;

    case 7:
      dayServo.userSetAngle();
      break;

    case 8:
      dayServo.angleCalibration();
      monthServo.angleCalibration();
      yearServo.angleCalibration();
      break;

    case 9:
      runAgeClock();
      break;

    default:
      Serial.print("Error: This value is not an option: ");
      Serial.println(input);
      delay(1000);
  }
}


void listenForGPSMessages() {
  ss.listen();
  delay(1500);
  bool clock_updated = false;

  while (!clock_updated) {

    if (!ss.available()){
      continue;
    }

    if(gps.encode(ss.read())) {

      if (!gps.date.isValid()) {
        Serial.println("Error: Date is not Valid.");
        continue;
      }

      if (gps.date.age() > 1500) {
        Serial.print("Warning: Age is old: ");
        Serial.println(gps.date.age());
        continue;
      }

      setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
      adjustTime(PDT_offset * SECS_PER_HOUR);

      updateClock();
      clock_updated = true;
    }
  }
  dummy_ss.listen();
}


void updateClock(){
  if (timeStatus()!= timeNotSet) {
    if (now() != prevDisplay) { //update the display only if the time has changed
      Serial.print("Time difference: ");
      Serial.println(now() - prevDisplay);
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
}


void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print("Current Time and Date: ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}


void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


void printMenu() {
  Serial.println();
  Serial.println("******* Menu *******");
  Serial.println("1. Test Age Calculator.");
  Serial.println("2. Print GPS Time data.");
  Serial.println("3. Use GPS to calculate Age.");
  Serial.println("4. Zero all Servos.");
  Serial.println("5. Center all Servos.");
  Serial.println("6. Sweep all Servos.");
  Serial.println("7. User Set Servo Position.");
  Serial.println("8. Calibrate Servos");
  Serial.println("9. Run Age Clock");
}


void runAgeClock() {
  while(true){
    listenForGPSMessages();
    calculateAge(day(), month(), year(), ageDay, ageMonth, ageYear);

    dayServo.updateAge(ageDay);
    monthServo.updateAge(ageMonth);
    yearServo.updateAge(ageYear);

    Serial.println();
    delay(10000);
  }

}
