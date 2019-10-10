#pragma once

#include "lights.h"
#include "rfid.h"

class Logic {
public:
  Logic();
  Lights lights;
  Rfid rfid;

  void setup();
  void handle();
  void solved();
  void unsolvable();
  void status();

private:
  bool _unsolvable = false;
};

