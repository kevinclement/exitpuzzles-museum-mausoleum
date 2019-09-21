#pragma once

#include "Arduino.h"

class Logic;

class Rfid {
  public:
    Rfid(Logic &logic);
    void setup();
    void handle();

    bool solved = false;
    bool rfidState[5] = {false};

  private:
    Logic &_logic;
    uint8_t getID(uint8_t reader);
    bool isIdol(byte id[], uint8_t reader);
    void printID(byte id[]);
    bool compareIDs(byte idOne[], byte idTwo[]);
    void checkForPuzzleSolved();
};