#include "Arduino.h"
#include "rfidReader.h"

RfidReader::RfidReader()
{
}

void RfidReader::setup(uint8_t pin, uint8_t rst) {

  // Pull Up SS
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(1);

  mfr.PCD_Init(pin, rst);
  Serial.print("Reader Pin: ");
  Serial.print(pin);
  Serial.print(" => ");
  mfr.PCD_DumpVersionToSerial();
}

bool RfidReader::check() {
  rfid_tag_present_prev = rfid_tag_present;

  _rfid_error_counter += 1;
  if(_rfid_error_counter > 2){
    _tag_found = false;
  }

  // Detect Tag without looking for collisions
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  // Reset baud rates
  mfr.PCD_WriteRegister(mfr.TxModeReg, 0x00);
  mfr.PCD_WriteRegister(mfr.RxModeReg, 0x00);

  // Reset ModWidthReg
  mfr.PCD_WriteRegister(mfr.ModWidthReg, 0x26);

  MFRC522::StatusCode result = mfr.PICC_RequestA(bufferATQA, &bufferSize);

  if(result == mfr.STATUS_OK){
    if ( ! mfr.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue   
      return;
    }
    _rfid_error_counter = 0;
    _tag_found = true;

    // TODO: copy
    // for ( uint8_t i = 0; i < 4; i++) {
    //   readCard[i] = mfr.uid.uidByte[i];
    // }
  }

  rfid_tag_present = _tag_found;
  
  // rising edge
  if (rfid_tag_present && !rfid_tag_present_prev){
    Serial.println("Tag found");
    // Serial.println("checking against known");
    // Serial.println(isIdol(readCard, idolIndex));
  }
  
  // falling edge
  if (!rfid_tag_present && rfid_tag_present_prev){
    Serial.println("Tag gone");
  }
}