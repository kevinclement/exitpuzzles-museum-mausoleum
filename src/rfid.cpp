#include "Arduino.h"
#include "rfid.h"
#include "logic.h"
#include "rfidReader.h"
#include <MFRC522.h>

#define RST_PIN    9

#define NR_OF_READERS   5
byte ssPins[] =  { 16, 17, 14, 10, 15 };

// RFID controller
MFRC522 mfrc522[NR_OF_READERS];
RfidReader rdr;

byte readCard[4];                                // Stores scanned ID read from RFID Module
byte masterCard[4] = { 0xA9, 0x9A, 0xBB, 0x55 }; // Stores master card's ID read from EEPROM

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
  // Pull Up SS lines
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    pinMode(ssPins[i], OUTPUT);  
    digitalWrite(ssPins[i], HIGH);
  }
  delay(1);

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    if (i == 2) {
      rdr.setup(ssPins[i], RST_PIN);
    } else {
      mfrc522[i].PCD_Init(ssPins[i], RST_PIN);
      Serial.print("Reader ");
      Serial.print(i);
      Serial.print(": Pin: ");
      Serial.print(ssPins[i]);
      Serial.print(" => ");
      mfrc522[i].PCD_DumpVersionToSerial();
    }
  }

  Serial.println("\nReady to Scan...");
}

void Rfid::handle() {
  

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    // TMP: IGNORE FOR TEST
    // DONT CHECK THIS IN
    if (i==2) {
      rdr.check();
      continue;
    }

    if (getID(i)) {
      if (isIdol(readCard, i)) {
        if (!solved) { 
          Serial.print("IDOL: ");
          Serial.println(i + 1);
          rfidState[i] = true;
          checkForPuzzleSolved();
          _logic.status();
        }
      } else { 
        Serial.print("UNKNOWN tag ");
        printID(readCard);
        Serial.print(" on reader ");
        Serial.print(i);
        Serial.println(".");
      }
    }
  }
}

uint8_t Rfid::getID(uint8_t reader) {
  // Getting ready for Reading PICCs
  if (!mfrc522[reader].PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522[reader].PICC_ReadCardSerial()) {
    return 0;
  }

  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522[reader].uid.uidByte[i];
  }

  // Stop reading
  mfrc522[reader].PICC_HaltA();
  mfrc522[reader].PCD_StopCrypto1();
  
  return 1;
}

bool Rfid::isIdol(byte id[], uint8_t reader) {
  return compareIDs(id, tags[reader]);
}

void Rfid::printID(byte id[]) {
  for ( uint8_t i = 0; i < 4; i++) {  //
    Serial.print(id[i] < 0x10 ? "0" : "");
    Serial.print(id[i], HEX);
  }
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
  bool allFound = true;
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    allFound = allFound && rfidState[i];
  }

  if (allFound) {
    solved = true;
  }
}