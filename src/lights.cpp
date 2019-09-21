#include "Arduino.h"
#include "lights.h"
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_TLC5947.h>

// LEDs didn't map proper to indexes, so I need a lookup
byte ledPins[]   = { 3, 4, 1, 0, 2 }; 
uint8_t ledColors[][3] = {
  { 255,   100,    50 }, // white
  {   0,     0,   255 }, // blue
  { 255,     0,     0 }, // red
  { 200,     0,   255 }, // purple
  {   0,   255,     0 }, // green
};

// LED pins
#define LED_DATA_PIN 4
#define LED_CLK_PIN  5
#define LED_LTCH_PIN 6

// LED controller
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLK_PIN, LED_DATA_PIN, LED_LTCH_PIN);

unsigned long blink_off = 0;
unsigned long blink_on = 0;
uint8_t blink_count = 0;

Lights::Lights(Logic &logic)
: _logic(logic)
{  
}

// void Lights::on() {
//   lights_on = true;
// }

// void Lights::allOn() {
//   ledcWrite(CHANNEL, 255);
// }

// void Lights::off() {
//   ledcWrite(CHANNEL, 0);
//   lights_on = false;
// }

void Lights::setup() {
  SPI.begin();
  tlc.begin();
}

void Lights::handle() {
  checkBlink();
  
  for (uint8_t i = 0; i < 5; i++) {
    if (blink_off > 0) {
      tlc.setLED(ledPins[i], 0, 0, 0);
    } else {
      tlc.setLED(ledPins[i], ledColors[i][0], ledColors[i][1], ledColors[i][2]);
    }
    tlc.write();
  }
}

void Lights::checkBlink() {
  uint16_t BLINK_DURATION = 500;
  uint8_t BLINK_MAX = 5;
  
  // only blink off for small ammount of time, then turn them back on  
  if (blink_off > 0 && millis() - blink_off > BLINK_DURATION) {
    Serial.println("blinking off");
    blink_off = 0;

    if (blink_count < BLINK_MAX - 1) {
      Serial.println("blinking back on");
      blink_on = millis();
      blink_count++;
    }
  }
  else if (blink_on > 0 && millis() - blink_on > BLINK_DURATION) {
    Serial.println("blink off set again");
    blink_on = 0;
    blink_off = millis();
  }
}

void Lights::blink() {
  blink_off = millis();
}
