#pragma once

#include "ClockConstants.h"

/// Age Calculator

int ageDay = 0;
int ageMonth = 0;
int ageYear = 0;


int lastMonthsNumDays(int currentMonth, int currentYear) {
  int monthDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // Check for leap year and adjust February days
  if ((currentYear % 4 == 0 && currentYear % 100 != 0) || (currentYear % 400 == 0)) {
    monthDays[1] = 29;  // February has 29 days in a leap year
  }

  // If current month is January, we return the days of December last year
  if (currentMonth == 1) {
    return monthDays[11]; // December of the previous year
  }
  else {
    return monthDays[currentMonth - 2]; // Previous month in the same year
  }
}


int lastLastMonthsNumDays(int currentMonth, int currentYear) {
  return lastMonthsNumDays(currentMonth - 1, currentYear);
}


void calculateAge(
    int inputDay, int inputMonth, int inputYear,
    int & ageDay, int & ageMonth, int & ageYear){
  int monthModifier = 0;
  int yearModifier = 0;

  int dayDiff = inputDay - birthDay;
  if (dayDiff >= 0) {
    ageDay = dayDiff;
  }
  else if (dayDiff < 0) {
    ageDay = lastMonthsNumDays(inputMonth, inputYear) + dayDiff;
    monthModifier += 1;

    if (ageDay < 0) {
      ageDay = lastLastMonthsNumDays(inputMonth, inputYear) + ageDay;
      monthModifier +=1;
    }
  }
  else {
    Serial.println("Error: dayDiff not a number?");
  }

  int monthDiff = inputMonth - birthMonth - monthModifier;
  if (monthDiff >= 0) {
    ageMonth = monthDiff;
  }
  else if (monthDiff < 0) {
    ageMonth = 12 + monthDiff;
    yearModifier += 1;
  }
  else {
    Serial.println("Error: monthDiff not a number?");
  }

  ageYear = inputYear - birthYear - yearModifier;

  Serial.print("Birthday: ");
  Serial.print(birthMonth);
  Serial.print(" / ");
  Serial.print(birthDay);
  Serial.print(" / ");
  Serial.println(birthYear);

  Serial.print("You are ");
  Serial.print(ageYear);
  Serial.print(" year(s), ");
  Serial.print(ageMonth);
  Serial.print(" month(s), and ");
  Serial.print(ageDay);
  Serial.println(" day(s) old.");

  Serial.println("");
}


void testAgeCalculater(){
  while(true){
    Serial.print("Outputs Age from Birth to Input Date.");
    Serial.print("Enter a Year: ");
    while (Serial.available() == 0) {}
    int testYear = Serial.parseInt();
    Serial.println(testYear);

    Serial.print("Enter a Month: ");
    while (Serial.available() == 0) {}
    int testMonth = Serial.parseInt();
    Serial.println(testMonth);

    Serial.print("Enter a Day: ");
    while (Serial.available() == 0) {}
    int testDay = Serial.parseInt();
    Serial.println(testDay);

    calculateAge(testDay, testMonth, testYear, ageDay, ageMonth, ageYear);

    delay(1000);

  }
}
