#pragma once

/// @class Servo class that for the clock
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

