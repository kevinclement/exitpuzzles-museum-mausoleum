#pragma once

#include "Arduino.h"
#include <MFRC522.h>

class RfidReader {
  public:
    RfidReader();
    void setup(uint8_t pin, uint8_t rst, byte t[4]);
    bool check();

  private:
    bool compareTags();
    void printID(byte id[]);
    uint8_t legacyGetID(uint8_t reader);

    MFRC522 mfr;
    byte tag[4];

    byte readCard[4];                                // Stores scanned ID read from RFID Module
    byte masterCard[4] = { 0xA9, 0x9A, 0xBB, 0x55 }; // Stores master card's ID read from EEPROM

    bool rfid_tag_present_prev = false;
    bool rfid_tag_present = false;
    int _rfid_error_counter = 0;
    bool _tag_found = false;
};