#include "Arduino.h"
#include "rfid.h"
#include "logic.h"
#include "rfidReader.h"
#include <MFRC522.h>

#define RST_PIN    9
byte ssPins[] =  { 16, 17, 14, 10, 15 };
RfidReader reader[NR_OF_READERS];

// TODO: should be stored in EEPROM and ability to update via serial
byte tags[][4] = {
  { 0xA7, 0x86, 0x8A, 0xF2 },
  { 0xF7, 0x81, 0x8A, 0xF2 }, 
  { 0x07, 0x83, 0x8A, 0xF2 },
  { 0xF7, 0x6F, 0x8C, 0xF2 },
  { 0x97, 0x6F, 0x8C, 0xF2 },
};

Rfid::Rfid(Logic &logic)
: _logic(logic)
{
}

void Rfid::setup() {  
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    reader[i].setup(ssPins[i], RST_PIN, tags[i]);
    state[i] = reader[i].state;
  }

  Serial.println("\nReady to Scan...");
}

void Rfid::handle() {

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {

    reader[i].handle();

    if (state[i] != reader[i].state) {
      Serial.print("state changed for ");
      Serial.print(i + 1);
      Serial.print(" ");
      Serial.print(prettyState(state[i]));
      Serial.print(" => ");
      Serial.println(prettyState(reader[i].state));

      state[i] = reader[i].state;
      checkForPuzzleSolved();
      _logic.status();
    }

    //     Serial.print("UNKNOWN tag ");
    //     printID(readCard);
    //     Serial.print(" on reader ");
    //     Serial.print(i);
    //     Serial.println(".");
    //   }
    // }
  }
}

bool Rfid::isIdol(byte id[], uint8_t reader) {
  return compareIDs(id, tags[reader]);
}

bool Rfid::compareIDs(byte idOne[], byte idTwo[] ) {   
  for ( uint8_t k = 0; k < 4; k++ ) {
    if ( idOne[k] != idTwo[k] ) {
       return false;
    }
  }
  return true;
}

void Rfid::checkForPuzzleSolved() {
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    solved = solved && state[i] == CORRECT;
  }
}

String Rfid::prettyState(uint8_t state) {
  return 
    state == INCORRECT ? "Incorrect" : 
    state == CORRECT ? "Correct" : 
    state == MISSING ? "Missing" : 
    "Unknown";
}