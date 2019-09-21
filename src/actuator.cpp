#include "Arduino.h"
#include "actuator.h"
#include "logic.h"

// motor control
#define PWMA 14
#define AIN1 15
#define AIN2 32
#define STBY 33

#define PRELOAD_TIME 5000
#define CLOSE_TIME 47000
#define OPEN_TIME 42000 // assumes preload

unsigned long preload_start = 0;
unsigned long close_start = 0;
unsigned long open_start = 0;

Actuator::Actuator(Logic &logic)
: _logic(logic)
{
}

void Actuator::setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
}

void Actuator::handle() {
  if (preload_start > 0 && millis() - preload_start > PRELOAD_TIME) {
    _logic.serial.print("preload finished.\n");
    preload_start = 0;
    stop();
    
    // since we use the open mechanism, we need to mark it not opened
    open_start = 0;
  }

  if (close_start > 0 && millis() - close_start > CLOSE_TIME) {
    opened = false;
    _logic.serial.print("closing finished.\n");
    close_start = 0;
    _logic.serial.print("Preloading for next open...\n");
    preload();
  }

  if (!opened && open_start > 0 && millis() - open_start > OPEN_TIME) {
    _logic.serial.print("opening finished.\n");
    opened = true;
  }
}

void Actuator::open() {
  open_start = millis();

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(PWMA, HIGH);
  digitalWrite(STBY, HIGH);
}

void Actuator::close() {
  close_start = millis();

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(PWMA, HIGH);
  digitalWrite(STBY, HIGH);
}

void Actuator::stop() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
}

void Actuator::preload() {
  preload_start = millis();
  open();
}
