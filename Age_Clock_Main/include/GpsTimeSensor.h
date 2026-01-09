#pragma once

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TinyGPSPlus.h>

#include "ClockConstants.h"
#include "ClockDate.h"

static constexpr bool is_testing = false;

class GpsTimeSensor {
 public:
  GpsTimeSensor()
  : ss{GPS_RX_PIN, GPS_TX_PIN},
    valid_date{false},
    is_updated{false},
    current_date{1900, 1, 1}
  {
    ss.begin(GPS_BAUD);
  }

  void listen() {
    // This sketch displays information every time a new sentence is correctly encoded.
    static long time_since_last_message = millis();

    while (ss.available())
    {
      gps.encode(ss.read());
    }

    if (gps.date.isUpdated())
    {
      is_updated = true;
      current_date = Date(gps.date.year(), gps.date.month(), gps.date.day());
      displayInfo();
    }

    if (millis() - time_since_last_message > 5000 && gps.charsProcessed() < 10) {
      Serial.println(F("ERROR: No GPS detected"));
      // Reset timer
      time_since_last_message = millis();
    }
  }

  // Check if the date is object using the gps class
  bool isValid() const {return gps.date.isValid();}

  // Check if the date has been updated
  bool isUpdated() const {return is_updated;}

  // Reset the update flag. Expected to be used after isUpdated returns true
  void resetUpdated() {is_updated = false;}

const Date getCurrentDate()
{
  return current_date;
}

void writeRawData()
{
  while (ss.available() > 0){
    byte gpsData = ss.read();
    Serial.write(gpsData);  // Write the raw data to the Serial Monitor
  }
}

 private:
  TinyGPSPlus gps;    // GPS Object
  SoftwareSerial ss;  // Serial Comms with the GPS object
  bool valid_date;    // Is the current date valid
  bool is_updated;    // Has the date been updated
  Date current_date;  // Current Date

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
    if (gps.date.isValid()) {
      Serial.print(gps.date.month());
      Serial.print(F("/"));
      Serial.print(gps.date.day());
      Serial.print(F("/"));
      Serial.print(gps.date.year());
    } else {
      Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
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
