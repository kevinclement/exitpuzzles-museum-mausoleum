#include "Arduino.h"
#include "lightsensors.h"
#include "logic.h"

int LS_ONE = 0;               // light sensor 1 reading
int LS_ONE_THRESH = 2500;
int LS_TWO = 0;               // light sensor 2 reading
int LS_TWO_THRESH = 1000;
int DEBOUNCE = 500;

bool reportedLight = false;
bool reportedCheat = false;
unsigned long light_first_seen = 0;
unsigned long light_two_first_seen = 0;
bool lightTwoDetected = false;

// light sensors
#define LS_ONE_PIN A3
#define LS_TWO_PIN A2

LightSensors::LightSensors(Logic &logic)
: _logic(logic)
{  
}

void LightSensors::setup() {
  pinMode(LS_ONE_PIN, INPUT);
  pinMode(LS_TWO_PIN, INPUT);
}

void LightSensors::handle() {
  LS_ONE = analogRead(LS_ONE_PIN);
  LS_TWO = analogRead(LS_TWO_PIN);

  if (debugReadings) {
    _logic.serial.print("1: %d 2: %d \n", LS_ONE, LS_TWO);
  }
  
  // Debounce sensor 2
  if (LS_TWO > LS_TWO_THRESH) {
    if (light_two_first_seen != 0) {
      if (millis() - light_two_first_seen > DEBOUNCE) {
        lightTwoDetected = true;
      }
    } else {
      light_two_first_seen = millis();
    }
  } else {
      light_two_first_seen = 0;
      lightTwoDetected = false;
  }

  // Debounce sensor 1
  if (LS_ONE > LS_ONE_THRESH) {
    // check for cheating
    if (lightTwoDetected) {
      if (!reportedCheat) {
        _logic.serial.print("CHEATER!\n");
        reportedCheat = true;
      }
      return;
    }

    if (light_first_seen != 0) {
      if (millis() - light_first_seen > DEBOUNCE) {
        if (!reportedLight) {
          _logic.serial.print("light detected\n");
          reportedLight = true;
        }
        lightDetected = true;
      }
    } else {
      light_first_seen = millis();
    }
  } else {
      light_first_seen = 0;
      reportedLight = false;
      lightDetected = false;
  }

  if (!lightDetected && !lightTwoDetected) {
    reportedCheat = false;
  }
}
