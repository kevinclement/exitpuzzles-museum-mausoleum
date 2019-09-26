#include "Arduino.h"
#include "logic.h"
#include "consts.h"
#include "rfidReader.h"
#include "version.h"

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
  char cMsg[254];
  sprintf(cMsg, 
    "status="
      "version:%s,"
      "gitDate:%s,"
      "buildDate:%s,"

      "solved:%s,"
      "idol_1:%s,"
      "idol_2:%s,"
      "idol_3:%s,"
      "idol_4:%s,"
      "idol_5:%s"

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      _solved ? "true" : "false",
      rfid.state[0] == CORRECT ? "true" : "false",
      rfid.state[1] == CORRECT ? "true" : "false",
      rfid.state[2] == CORRECT ? "true" : "false",
      rfid.state[3] == CORRECT ? "true" : "false",
      rfid.state[4] == CORRECT ? "true" : "false",

      CRLF);

  Serial.print(cMsg);
}
