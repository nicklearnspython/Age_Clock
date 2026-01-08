#pragma once

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

#include "ClockConstants.h"

class GpsTimeSensor {
 public:
  GpsTimeSensor()
  {
    ss.begin(GPS_BAUD);
    ss.listen();
  }

  bool listen() {
    // This sketch displays information every time a new sentence is correctly encoded.
    static long time_since_last_message = millis();
    while (ss.available() > 0)
    {
      //char gps_data = ss.read();  // Read a byte of data
      //Serial.write(gps_data);     // Write that byte to the hardware serial port (Serial Monitor)
      if (gps.encode(ss.read()))
      {
        displayInfo();
        return true;
      }
    }

    //if (millis() - time_since_last_message > 5000 && gps.charsProcessed() < 10) {
    //  Serial.println(F("ERROR: No GPS detected"));
    //  //while (true);
    //}

    return false;
  }

 private:
  TinyGPSPlus gps;
  SoftwareSerial ss{GPS_RX_PIN, GPS_TX_PIN};

  void displayInfo() {
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print(gps.location.lng(), 6);
    } else {
      Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isUpdated()) {
      Serial.print(gps.date.month());
      Serial.print(F("/"));
      Serial.print(gps.date.day());
      Serial.print(F("/"));
      Serial.print(gps.date.year());
    } else {
      Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isUpdated()) {
      if (gps.time.hour() < 10) Serial.print(F("0"));
      Serial.print(gps.time.hour());
      Serial.print(F(":"));
      if (gps.time.minute() < 10) Serial.print(F("0"));
      Serial.print(gps.time.minute());
      Serial.print(F(":"));
      if (gps.time.second() < 10) Serial.print(F("0"));
      Serial.print(gps.time.second());
      Serial.print(F("."));
      if (gps.time.centisecond() < 10) Serial.print(F("0"));
      Serial.print(gps.time.centisecond());
    } else {
      Serial.print(F("INVALID"));
    }

    Serial.println();
  }
};
