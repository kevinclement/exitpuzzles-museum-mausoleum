#include "Arduino.h"
#include "sound.h"
#include <HardwareSerial.h>

void sendCommand(int8_t command, int16_t dat);

HardwareSerial MySerial(1);
static int8_t Send_buf[8] = {0};

Sound::Sound(Logic &logic)
: _logic(logic)
{  
}

void Sound::setup() {
  MySerial.begin(9600, SERIAL_8N1, 16, 17);
  delay(500);              //Wait chip initialization is complete
  sendCommand(0X09, 0X02); // select the TF card
  delay(200);
}

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    MySerial.write(Send_buf[i]);
  }
}

void Sound::play() {
  sendCommand(0X22, 0X4601);
}

void Sound::stop() {
  sendCommand(0X16, 0X00);
}



