#include "Arduino.h"
#include "logic.h"
#include "consts.h"

unsigned long solved_at = 0;
bool stopped_all = false;

// Logic::Logic() 
//   : serial(),
//     rfid(*this),
//     lightsensors(*this),
//     sound(*this)
// {
// }

Logic::Logic() : 
  lights(*this),
  rfid(*this)
{
}

bool _solved = false;
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

  // lightsensors.handle();

  // if (lightsensors.lightDetected && solved_at == 0) {
  //   solved();
  // }

  
    // serial.print("All done with lights and music!\n");
    // sound.stop();
    // lights.off();
    // stopped_all = true;
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
