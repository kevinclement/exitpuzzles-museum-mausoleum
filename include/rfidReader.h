#pragma once

#include "Arduino.h"
#include <MFRC522.h>

class RfidReader {
  public:
    RfidReader();
    void setup(uint8_t pin, uint8_t rst);
    bool check();

  private:
    MFRC522 mfr;
    bool rfid_tag_present_prev = false;
    bool rfid_tag_present = false;
    int _rfid_error_counter = 0;
    bool _tag_found = false;
};