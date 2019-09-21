#include "Arduino.h"
#include "logic.h"
#include "consts.h"
#include "rfidReader.h"

bool _solved = false;
Logic::Logic() : 
  lights(*this),
  rfid(*this)
{
}

void Logic::setup() {
  lights.setup();
  rfid.setup();
}

void Logic::handle() {
  lights.handle();
  rfid.handle();

  if (rfid.solved && !_solved) {
    solved();
  }
}

void Logic::solved() {
  Serial.println("*** ALL IDOLS IN PLACE ***");
  lights.blink();
  _solved = true;
  status();
}

void Logic::status() {
  Serial.print("status=solved:");
  Serial.print(_solved ? "true" : "false");
  Serial.print(",idol_1:");
  Serial.print(rfid.state[0] == CORRECT ? "true" : "false");
  Serial.print(",idol_2:");
  Serial.print(rfid.state[1] == CORRECT ? "true" : "false");
  Serial.print(",idol_3:");
  Serial.print(rfid.state[2] == CORRECT ? "true" : "false");
  Serial.print(",idol_4:");
  Serial.print(rfid.state[3] == CORRECT ? "true" : "false");
  Serial.print(",idol_5:");
  Serial.print(rfid.state[4] == CORRECT ? "true" : "false");
  Serial.println();
}
