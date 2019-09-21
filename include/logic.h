#pragma once

#include "serialmanager.h"
#include "rfid.h"
#include "lights.h"
#include "lightsensors.h"
#include "sound.h"

class Logic {
public:
  Logic();
  SerialManager serial;
  Rfid rfid;
  Lights lights;
  LightSensors lightsensors;
  Sound sound;

  void setup();
  void handle();
  void solved();
  void status();

private:
  
};

