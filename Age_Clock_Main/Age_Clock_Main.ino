/*
 * Nick Garrett
 * The Age Clock
 */

#include <TimeLib.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Servo.h>

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


class ClockServo {
private:
  Servo clockServo;     // Servo instance
  String title;
  int pin;              
  int angleMin;         
  int angleMax;         
  int ageMin;
  int ageMax;
  int pos;
  int cal_input;
  uint8_t positions[32];
  
public:
  ClockServo(String servoTitle, int servoPin, int minAngle, int maxAngle, int minAge, int maxAge)
    : title(servoTitle), pin(servoPin), angleMin(minAngle), angleMax(maxAngle), ageMin(minAge), ageMax(maxAge) {
      if (title == "Day") {
        memcpy(positions, dayAngles, 31+1);
      }
      else if (title == "Month") {
        memcpy(positions, monthAngles, 12+1);
      }
      else if (title == "Year") {
        memcpy(positions, yearAngles, 17+1);
      }
    }

  
  void servoAttach() {
    clockServo.attach(pin);
  }
  

  void servoDetach() {
    clockServo.detach();
  }
  

  void setAngle(int angle) {
    Serial.print(title);
    Serial.print(".setAngle() - ");
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

  int ageToAngle(int newAge) {
    int angle = positions[newAge];
    if(angle == NULL) {
      Serial.println("Error: NULL position");
      angle = 0;
    }
    return angle;
  }
  

  void updateAge(int newAge){
    servoAttach();
    setAngle(ageToAngle(newAge));
    servoDetach();
  }

  void center(){
    servoAttach();
    setAngle(90);
    servoDetach();
  }

  void zero(){
    servoAttach();
    setAngle(0);
    servoDetach();
  }

  void sweep(){
    servoAttach();
    int newAge = ageMin;
    
    for (newAge = ageMin; newAge <= ageMax; newAge += 1) {
      Serial.print("Age: ");
      Serial.println(newAge);
      setAngle(ageToAngle(newAge));
      delay(500); 
    }
    delay(1000);
    center();
    delay(1000);
    servoDetach();
  }

  void userSetAngle(){
    while(true){
      while (Serial.available() == 0) {}
      cal_input = Serial.parseInt();
      
      servoAttach();
      setAngle(cal_input);
      servoDetach();
    }
  }

  void angleCalibration(){
    Serial.println("Calibrate ");
    Serial.print(title);
    Serial.println(" age to servo angle.");
    int index = 0;
    
    while(true){
      Serial.print("Age: ");
      Serial.print(index);
      Serial.print(", Servo angle: ");
      
      while (Serial.available() == 0) {}
      cal_input = Serial.parseInt();

      Serial.println(cal_input);

      if(cal_input != -1){
        Serial.println("Calibration complete.");
        break;
      }
      
      positions[index] = cal_input;
      index++;
    }
  }
};


ClockServo dayServo(dayServoTitle, dayServoPin, dayServoAngleMin, dayServoAngleMax, daysMin, daysMax);
ClockServo monthServo(monthServoTitle, monthServoPin, monthServoAngleMin, monthServoAngleMax, monthsMin, monthsMax);
ClockServo yearServo(yearServoTitle, yearServoPin, yearServoAngleMin, yearServoAngleMax, yearsMin, yearsMax); 

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
      calculateAge(day(), month(), year());
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
    calculateAge(day(), month(), year());
    
    dayServo.updateAge(ageDay);
    monthServo.updateAge(ageMonth);
    yearServo.updateAge(ageYear);

    Serial.println();
    delay(10000);
  }
  
}
