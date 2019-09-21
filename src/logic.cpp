#include "Arduino.h"
#include "logic.h"
#include "consts.h"

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
  Serial.println();
}
