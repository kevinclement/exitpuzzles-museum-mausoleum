#include "Arduino.h"
#include "logic.h"
#include "consts.h"

unsigned long solved_at = 0;
bool stopped_all = false;

Logic::Logic() 
  : serial(*this),
    actuator(*this),
    lights(*this),
    lightsensors(*this),
    sound(*this)
{
}

void Logic::setup() {
  serial.setup();
  actuator.setup();
  lights.setup();
  lightsensors.setup();
  sound.setup();

  readStoredVariables();

  serial.printHelp();
  printVariables();
}

void Logic::handle() {
  serial.handle();
  lightsensors.handle();
  lights.handle();
  actuator.handle();

  if (lightsensors.lightDetected && solved_at == 0) {
    solved();
  }

  if (!stopped_all && solved_at != 0 && actuator.opened) {
    serial.print("All done with lights and music!\n");
    sound.stop();
    lights.off();
    stopped_all = true;
  }
}

void Logic::solved() {
  serial.print("Solved!\n");

  sound.play();
  lights.on();
  actuator.open();

  solved_at = millis();
}

void Logic::readStoredVariables() {
  //EEPROM.begin(64); // don't need a big size
  //EEPROM.get(FOO_VAR_ADDR, FOO_VAR);
}

void Logic::printVariables() { 
  serial.print(CRLF);
  serial.print("Current Variables:%s", CRLF);
}