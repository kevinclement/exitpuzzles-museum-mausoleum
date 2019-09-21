#pragma once

#include "Arduino.h"

class Logic;

class Lights {
  public:
    Lights(Logic &logic);
    void setup();
    void handle();

    // void on();
    // void off();
    // void allOn();

  private:
    Logic &_logic;
};