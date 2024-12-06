#ifndef lock_door_h
#define lock_door_h

#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

//define cac chan cua esp32
#define PIN_SG90 17 //pin servo
#define SS_PIN 5  // save slave in rfid
#define RST_PIN 0 // reset in rfid
#define LED 13
#define BUZZER 4 //buzzer pin

extern unsigned char index_t;

extern char password[6];
extern char pass_default[6];

extern unsigned char in_num, time_error, isMode;
extern LiquidCrystal_I2C lcd;

//rfid
extern MFRC522 rfid;

//servo
extern Servo sg90;

void writeEpprom(char data[]);
void readEpprom();
void checkPass();
void rfidCheck();
void changePass();
void resetPass();
void openDoor();
void errored3Times();
void errored5Times();
void addRFID();
void delRFID();
void delAllRFID();

#endif // LOCK_DOOR_H