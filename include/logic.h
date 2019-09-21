#pragma once

#include "SerialManager.h"
#include "actuator.h"
#include "lights.h"
#include "lightsensors.h"
#include "sound.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  Actuator actuator;
  Lights lights;
  LightSensors lightsensors;
  Sound sound;

  void setup();
  void handle();
  void solved();
  void status();

private:
  
};

