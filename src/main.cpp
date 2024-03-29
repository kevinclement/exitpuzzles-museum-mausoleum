#include <Arduino.h>
#include "logic.h"
#include "consts.h"

Logic logic;

void(* resetFunc) (void) = 0;

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  // read and handle message from serial
  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  if (msg == "blink" || msg == "b") {
    Serial.println("blinking lights now...");
    logic.lights.blink();
  }
  else if (msg == "solve" || msg == "v") {
    logic.solved();
  }
  else if (msg == "reset" || msg == "reboot" || msg == "r") {
    resetFunc();
  } else {
    Serial.print("unknown command: ");
    Serial.println(msg);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  Serial.println("Museum mausoleum idol control by kevinc...\n");

  logic.setup();
  logic.status();
}

void loop() {
  readAnySerialMessage();
  logic.handle();
}
