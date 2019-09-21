#include "Arduino.h"
#include "rfid.h"
#include "logic.h"
#include <MFRC522.h>

#define RST_PIN    9

#define NR_OF_READERS   5
byte ssPins[] =  { 16, 17, 14, 10, 15 };

// RFID controller
MFRC522 mfrc522[NR_OF_READERS];

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
    mfrc522[i].PCD_Init(ssPins[i], RST_PIN);
    Serial.print("Reader ");
    Serial.print(i);
    Serial.print(": Pin: ");
    Serial.print(ssPins[i]);
    Serial.print(" => ");
    mfrc522[i].PCD_DumpVersionToSerial();
  }

  Serial.println("\nReady to Scan...");
}

bool rfid_tag_present_prev = false;
bool rfid_tag_present = false;
int _rfid_error_counter = 0;
bool _tag_found = false;
void Rfid::checkForIdol(uint8_t idolIndex) {

  rfid_tag_present_prev = rfid_tag_present;

  _rfid_error_counter += 1;
  if(_rfid_error_counter > 2){
    _tag_found = false;
  }

  // Detect Tag without looking for collisions
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  // Reset baud rates
  mfrc522[idolIndex].PCD_WriteRegister(mfrc522[idolIndex].TxModeReg, 0x00);
  mfrc522[idolIndex].PCD_WriteRegister(mfrc522[idolIndex].RxModeReg, 0x00);

  // Reset ModWidthReg
  mfrc522[idolIndex].PCD_WriteRegister(mfrc522[idolIndex].ModWidthReg, 0x26);

  MFRC522::StatusCode result = mfrc522[idolIndex].PICC_RequestA(bufferATQA, &bufferSize);

  if(result == mfrc522[idolIndex].STATUS_OK){
    if ( ! mfrc522[idolIndex].PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue   
      return;
    }
    _rfid_error_counter = 0;
    _tag_found = true;

    for ( uint8_t i = 0; i < 4; i++) {
      readCard[i] = mfrc522[idolIndex].uid.uidByte[i];
    }
  }

  rfid_tag_present = _tag_found;
  
  // rising edge
  if (rfid_tag_present && !rfid_tag_present_prev){
    Serial.println("Tag found");
    Serial.println("checking against known");
    Serial.println(isIdol(readCard, idolIndex));
  }
  
  // falling edge
  if (!rfid_tag_present && rfid_tag_present_prev){
    Serial.println("Tag gone");
  }
}

void Rfid::handle() {
  

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    // TMP: IGNORE FOR TEST
    // DONT CHECK THIS IN
    if (i==2) {
      checkForIdol(i);
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