#include <Arduino.h>
#include "logic.h"
#include "consts.h"

// Globals
Logic logic;

void reboot(int) {
  ESP.restart();
}

void status(int) {
  logic.status();
}

void solve(int) {
  logic.serial.print("forcing a puzzle solve...%s", CRLF);
  logic.solved();
}

void debug(int) {
  logic.serial.print("toggling light sensor debugging...%s", CRLF);
  logic.lightsensors.debugReadings = !logic.lightsensors.debugReadings;
}

void setup() {
  logic.setup();
  logic.serial.print("Museum Mummy by kevinc...\n");

  logic.serial.registerCommand(SerialCommand("status",     's', &status,     "status",     "gets the status of device"));
  logic.serial.registerCommand(SerialCommand("solve",      'v', &solve,      "solve",      "force a puzzle solve of the device"));
  logic.serial.registerCommand(SerialCommand("debug",      'x', &debug,      "debug",      "debug sensors"));
  logic.serial.registerCommand(SerialCommand("reboot",     'r', &reboot,     "reboot",     "software reboot the device"));

  logic.serial.printHelp();
  logic.status();
}

void loop() {
  logic.handle();
}

//print("Opening device...%s", CRLF);
//print("Closing device...%s", CRLF);
//print("Turning lights on...%s", CRLF);
//print("Turning all lights on...%s", CRLF);
//print("Turning lights off...%s", CRLF);
// print("Playing sound track...%s", CRLF);
// print("Stoping sound track...%s", CRLF);
// print("Stoping actuator...%s", CRLF);
// print("Preloading actuator...%s", CRLF);
// print("Toggling debug mode...%s", CRLF);
// print("Triggering solved...%s", CRLF);

// PREVIOUS COMMANDS ---------------------------
// ---------------------------------------------
//  open,o       => logic.actuator.open()
//  close,c      => logic.actuator.close()
//  lightson,l   => logic.lights.on()
//  lightsall,a  => logic.lights.allOn()
//  lightsoff,f  => logic.lights.off()
//  play,p       => logic.sound.play()
//  playstop,y   => logic.sound.stop()
//  acstop,t     => logic.actuator.stop()
//  preload,e    => logic.actuator.preload()
//  solved,z     => logic.solved()
//  status,s     => logic.printVariables()
