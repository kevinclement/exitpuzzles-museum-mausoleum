#pragma once

#include "lights.h"
#include "rfid.h"

// #include "lightsensors.h"
// #include "sound.h"

class Logic {
public:
  Logic();
  Lights lights;
  Rfid rfid;

  // LightSensors lightsensors;
  // Sound sound;

  void setup();
  void handle();
  void solved();
  void status();

private:
  
};

