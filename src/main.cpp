#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_TLC5947.h>

// RFID pins
#define SS_PIN_1  16
#define SS_PIN_2  17
#define SS_PIN_3  14
#define SS_PIN_4  10
#define SS_PIN_5  15

#define RST_PIN    9

void checkBlink();
void resetAll();
void blink();
void checkForPuzzleSolved();
uint8_t getID(uint8_t reader);
bool isMaster(byte id[]);
bool isIdol(byte id[], uint8_t reader);
void printID(byte id[]);
bool compareIDs(byte idOne[], byte idTwo[] );

#define NR_OF_READERS   5
byte ssPins[] =  { SS_PIN_1, SS_PIN_2, SS_PIN_3, SS_PIN_4, SS_PIN_5 };

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

// RFID controller
MFRC522 mfrc522[NR_OF_READERS];
bool rfidState[] = { false, false, false, false, false };

// Magnet
#define MAGNET_PIN 3

// LED controller
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLK_PIN, LED_DATA_PIN, LED_LTCH_PIN);

byte readCard[4];                                // Stores scanned ID read from RFID Module
byte masterCard[4] = { 0xA9, 0x9A, 0xBB, 0x55 }; // Stores master card's ID read from EEPROM
bool unlocked = false;
unsigned long blink_off = 0;
unsigned long blink_on = 0;
uint8_t blink_count = 0;

// TMP: TODO: store in EEPROM
byte tags[][4] = {
  { 0xA7, 0x86, 0x8A, 0xF2 },
  { 0xF7, 0x81, 0x8A, 0xF2 }, 
  { 0x07, 0x83, 0x8A, 0xF2 },
  { 0xF7, 0x6F, 0x8C, 0xF2 },
  { 0x97, 0x6F, 0x8C, 0xF2 },
};

byte original_tags[][4] = {
  { 0x89, 0x72, 0xA1, 0x55 },
  { 0x52, 0xB4, 0x25, 0x1C }, 
  { 0x12, 0x4B, 0x57, 0x1C },
  { 0x09, 0x20, 0x92, 0x55 },
  { 0x76, 0x61, 0x93, 0x1A },
};

// TODO: 
//   [ ] unlock door wired
//   [ ] play mp3
//   [ ] install speaker
//   [ ] program other pedastool to show color of idol
//   [ ] bug with reset when they are all on the pedastal

//   [ ] master card programming mode

void setup() {
  Serial.begin(9600);
  Serial.println("Mausoleum idol control by kevinc...\n");

  // Pull Up SS lines
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    pinMode(ssPins[i], OUTPUT);  
    digitalWrite(ssPins[i], HIGH);
  }
  delay(1);
  
  SPI.begin();
  tlc.begin();

  pinMode(MAGNET_PIN, OUTPUT);

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    //mfrc522[i].PCD_SetAntennaGain(mfrc522[i].RxGain_max);
    mfrc522[i].PCD_Init(ssPins[i], RST_PIN);
    Serial.print("Reader ");
    Serial.print(i);
    Serial.print(": Pin: ");
    Serial.print(ssPins[i]);
    Serial.print(" => ");
    mfrc522[i].PCD_DumpVersionToSerial();
  }

  Serial.println("\nReady to Scan...");
}

void readAnySerialMessage() {
  if (!Serial.available()) {
    return;
  }

  // read and handle message from serial
  String msg = Serial.readStringUntil('\n');
  Serial.print("got '");
  Serial.print(msg);
  Serial.println("' command");

  String command = msg;
  int value = -1;

  // check if we need to split on space for advance commands
  for (int i = 0; i <= msg.length(); i++) {
      if (msg.charAt(i) == ' ') {         
          command = msg.substring(0, i);
          value = msg.substring(i+1, msg.length()).toInt();
      }
  }
 
  if (command == "blink" || command == "b") {
    Serial.println("blinking lights now...");
    blink();
  }
  else if (command == "reset") {
    Serial.println("resetting device now...");
    resetAll();
  }
}

void loop() {
  readAnySerialMessage();

  digitalWrite(MAGNET_PIN, unlocked ? LOW : HIGH);

  checkBlink();

  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    if (blink_off > 0) {
      tlc.setLED(ledPins[i], 0, 0, 0);
    } else {
      tlc.setLED(ledPins[i], ledColors[i][0], ledColors[i][1], ledColors[i][2]);
    }
    tlc.write();
    
    if (getID(i)) {
      if (isMaster(readCard)) {       
        Serial.print("MASTER from reader ");
        Serial.println(i);
        resetAll();      
      } else if (isIdol(readCard, i)) {
        if (!unlocked) { 
          Serial.print("IDOL: ");
          Serial.println(i + 1);
          rfidState[i] = true;
          checkForPuzzleSolved();
        }
      } else { 
        Serial.print("UNKNOWN tag ");
        printID(readCard);
        Serial.print(" on reader ");
        Serial.print(i);
        Serial.println(".");
      }
    }
  }
}

void checkBlink() {
  uint8_t BLINK_DURATION = 500;
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

void resetAll() {
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {   
    rfidState[i] = false;
  }
  
  unlocked = false;
  blink_on = 0;
  blink_off = 0;
  blink_count = 0;
}

void blink() {
  blink_off = millis();
}

void checkForPuzzleSolved() {
  bool solved = true;
  for (uint8_t i = 0; i < NR_OF_READERS; i++) {
    solved = solved && rfidState[i];
  }

  if (solved) {
    Serial.println("*** ALL IDOLS IN PLACE, OPENING LOCK ***");
    blink();
    unlocked = true;
  }
}

uint8_t getID(uint8_t reader) {
  // Getting ready for Reading PICCs
  if (!mfrc522[reader].PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522[reader].PICC_ReadCardSerial()) {
    return 0;
  }

  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522[reader].uid.uidByte[i];
  }

  // Stop reading
  mfrc522[reader].PICC_HaltA();
  mfrc522[reader].PCD_StopCrypto1();
  
  return 1;
}

bool isMaster(byte id[]) {
  return compareIDs(id, masterCard);
}

bool isIdol(byte id[], uint8_t reader) {
  return compareIDs(id, tags[reader]);
}

void printID(byte id[]) {
  for ( uint8_t i = 0; i < 4; i++) {  //
    Serial.print(id[i] < 0x10 ? "0" : "");
    Serial.print(id[i], HEX);
  }
}

bool compareIDs(byte idOne[], byte idTwo[] ) {   
  for ( uint8_t k = 0; k < 4; k++ ) {
    if ( idOne[k] != idTwo[k] ) {
       return false;
    }
  }
  return true;  
}