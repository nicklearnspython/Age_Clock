/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>

static const int RXPin = 4, TXPin = 3;
static const int dummy_RXPin = 13, dummy_TXPin = 12;
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

static const int dayServoPin = 11;
static const int monthServoPin = 10;
static const int yearServoPin = 9;

static const int daysMin = 0;
static const int daysMax = 30;
static const int monthsMin = 0;
static const int monthsMax = 11;
static const int yearsMin = 0;
static const int yearsMax = 18;

static const int dayServoAngleMin = 20;
static const int dayServoAngleMax = 160;
static const int monthServoAngleMin = 20;
static const int monthServoAngleMax = 160;
static const int yearServoAngleMin = 20;
static const int yearServoAngleMax = 160;

// Offset hours from gps time (UTC)
static const int CST_offset = -6;  // Central Standard Time (USA)
static const int CDT_offset = -5;  // Central Daylight Time (USA)
static const int PST_offset = -8;  // Pacific Standard Time (USA)
static const int PDT_offset = -7;  // Pacific Daylight Time (USA)


class ClockServo {
private:
  Servo clockServo;     // Servo instance
  int pin;              
  int angleMin;         
  int angleMax;         
  int ageMin;
  int ageMax;
  int pos;

public:
  ClockServo(int servoPin, int minAngle, int maxAngle, int minAge, int maxAge)
    : pin(servoPin), angleMin(minAngle), angleMax(maxAngle), ageMin(minAge), ageMax(maxAge) {}

  
  void servoAttach() {
    clockServo.attach(pin);
  }
  

  void servoDetach() {
    clockServo.detach();
  }
  

  void setAngle(int angle) {
    if (angle >= angleMin && angle <= angleMax) {
      Serial.print("Setting angle: ");
      Serial.println(angle);
    }
    else if (angle < angleMin) {
      Serial.print("Warning: angle set below minimum. Setting to angleMin. Set angle: ");
      Serial.println(angle);
      angle = angleMin;
    }
    else if (angle > angleMax) {
      Serial.print("Warning: angle set above maximum. Setting to angleMax. Set angle: ");
      Serial.println(angle);
      angle = angleMax;
    }
    else {
      Serial.println("Error: Set Angle Failure.");
      return;
    }

    clockServo.write(angle);
    delay(500);
  }
  

  void updateAge(int newAge){
    servoAttach();
    setAngle(map(newAge, ageMin, ageMax, angleMin, angleMax));
    servoDetach();
  }

  void center(){
    servoAttach();
    setAngle(90);
    servoDetach();
  }

  void testServo(){
    servoAttach();
    int newAge = ageMin;
    
    for (newAge = ageMin; newAge <= ageMax; newAge += 1) {
      Serial.print("Age: ");
      Serial.println(newAge);
      setAngle(map(newAge, ageMin, ageMax, angleMin, angleMax));
      delay(500); 
    }
    delay(1000);
    center();
    delay(1000);
    servoDetach();
  }
};


ClockServo dayServo(dayServoPin, dayServoAngleMin, dayServoAngleMax, daysMin, daysMax);
ClockServo monthServo(monthServoPin, monthServoAngleMin, monthServoAngleMax, monthsMin, monthsMax);
ClockServo yearServo(yearServoPin, yearServoAngleMin, yearServoAngleMax, yearsMin, yearsMax); 

TinyGPSPlus gps; 
SoftwareSerial ss(RXPin, TXPin);                    // The serial connection to the GPS device
SoftwareSerial dummy_ss(dummy_RXPin, dummy_TXPin);  // The serial connection to a dummy serial device. This prevents interrupts to servo control
time_t prevDisplay = 0;

int input = 0;
int ageDay = 0;
int ageMonth = 0;
int ageYear = 0;


void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
  dummy_ss.begin(GPSBaud);
  
  Serial.println("");
  Serial.println("------- Age Clock Initiate -------");
}


void loop(){
  printMenu();
  while (Serial.available() == 0) {}
  input = Serial.parseInt();
  
  Serial.print("You inputed the value: ");
  Serial.println(input);

  switch (input) {
    
    case 1:
      listenForGPSMessages();
      break;
      
    case 2: 
      dayServo.testServo();
      monthServo.testServo();
      yearServo.testServo();
      break;

    case 3:
      //moveDayServo();
      //moveMonthServo();
      //moveYearServo();
      break;

    case 4: 
      testAgeCalculater();
      break;

    case 5:
      listenForGPSMessages();
      calculateAge(day(), month(), year());
      break;

    case 6:
      dayServo.center();
      monthServo.center();
      yearServo.center();
      break;

    case 7:
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


void updateAge(){
  calculateAge(day(), month(), year());
}


void testAgeCalculater(){
  while(true){
    Serial.print("Enter Test Year: "); 
    while (Serial.available() == 0) {}
    int testYear = Serial.parseInt();
    Serial.println(testYear);

    Serial.print("Enter Test Month: "); 
    while (Serial.available() == 0) {}
    int testMonth = Serial.parseInt();
    Serial.println(testMonth);
    
    Serial.print("Enter Test Day: "); 
    while (Serial.available() == 0) {}
    int testDay = Serial.parseInt();
    Serial.println(testDay);

    calculateAge(testDay, testMonth, testYear);

    delay(1000);
    
  }
}


void calculateAge(int inputDay, int inputMonth, int inputYear){
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

  int ageYear = inputYear - birthYear - yearModifier;

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
  Serial.println("1. Print GPS Time data.");
  Serial.println("2. Test all servos.");
  Serial.println("3. Nothing...");
  Serial.println("4. Test Age Calculator.");
  Serial.println("5. Use GPS to calculate Age.");
  Serial.println("6. Center all Servos.");
  Serial.println("7. Run Age Clock");
  
}


void runAgeClock() {
  while(true){
    listenForGPSMessages();
    calculateAge(day(), month(), year());
    
    dayServo.updateAge(ageDay);
    monthServo.updateAge(ageMonth);
    yearServo.updateAge(ageYear);

    Serial.println();
    delay(10000);
  }
  
}