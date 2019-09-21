#pragma once

#include "Arduino.h"
#include "rfidReader.h"

class Logic;

#define NR_OF_READERS   5

class Rfid {
  public:
    Rfid(Logic &logic);
    void setup();
    void handle();
    bool solved = false;
    RFID_STATE state[NR_OF_READERS];

  private:
    Logic &_logic;
    uint8_t getID(uint8_t reader);
    bool isIdol(byte id[], uint8_t reader);
    bool compareIDs(byte idOne[], byte idTwo[]);
    void checkForPuzzleSolved();
};