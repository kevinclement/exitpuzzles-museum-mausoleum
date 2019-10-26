#pragma once

#include "lights.h"

class Logic {
public:
  Logic();
  Lights lights;

  void setup();
  void handle();
  void solved();
  void status();

private:
  bool _solved = false;
};

