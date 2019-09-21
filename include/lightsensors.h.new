#pragma once

#include "Arduino.h"

class Logic;

class LightSensors {
  public:
    LightSensors(Logic &logic);
    void setup();
    void handle();

    bool lightDetected = false;
    bool debugReadings = false;

  private:
    Logic &_logic;
};