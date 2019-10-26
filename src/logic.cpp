#include "Arduino.h"
#include "logic.h"
#include "consts.h"
#include "version.h"

Logic::Logic() : 
  lights(*this)
{
}

void Logic::setup() {
  lights.setup();
}

void Logic::handle() {
  lights.handle();
}

void Logic::solved() {
  Serial.println("*** SOLVED ***");
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

      "%s"
    , GIT_HASH,
      GIT_DATE,
      DATE_NOW,

      _solved ? "true" : "false",

      CRLF);

  Serial.print(cMsg);
}
