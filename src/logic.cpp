#include "Arduino.h"
#include "logic.h"
#include "consts.h"
#include <SPI.h>

unsigned long solved_at = 0;
bool stopped_all = false;

// Logic::Logic() 
//   : serial(),
//     rfid(*this),
//     lights(*this),
//     lightsensors(*this),
//     sound(*this)
// {
// }

Logic::Logic() : lights(*this)
{
}

void Logic::setup() {
  // serial.setup("");

  // rfid.setup();
  // lights.setup();
  // lightsensors.setup();
  // sound.setup();
}

void Logic::handle() {
  // serial.handle();
  // lightsensors.handle();
  // lights.handle();
  // rfid.handle();

  // if (lightsensors.lightDetected && solved_at == 0) {
  //   solved();
  // }

  
    // serial.print("All done with lights and music!\n");
    // sound.stop();
    // lights.off();
    // stopped_all = true;
}

void Logic::solved() {
  // serial.print("Solved!\n");

  // sound.play();
  // lights.on();

  // solved_at = millis();
  // status();
}

void Logic::status() {
  // serial.print(
  //   "status=solved:%s"
  //   "%s",
  //   solved_at > 0 ? "true" : "false",
  //   CRLF);
}
