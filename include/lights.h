#pragma once

#include "Arduino.h"

class Logic;

class Lights {
  public:
    Lights(Logic &logic);
    void setup();
    void handle();

    void blink();

  private:
    Logic &_logic;
    void checkBlink();
};