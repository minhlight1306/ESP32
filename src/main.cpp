#include <Arduino.h>

//include tung file
#include <lock_door.h>
#include "sensor.h"

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(512);
    sg90.setPeriodHertz(50);
    sg90.attach(PIN_SG90, 500, 2400);
    SPI.begin();
    rfid.PCD_Init();
//init lcd
    lcd.init();
    lcd.backlight();
    lcd.print("   SYSTEM INIT   ");
    readEpprom();
    delay(2000);
    lcd.clear();
    Serial.print("PASSWORD: ");
    Serial.println(password);
//pin mode
    pinMode(LED, OUTPUT);
    pinMode(fire_sensor, INPUT);
    pinMode(gas_sensor, INPUT);
    pinMode(D0_light_sensor, INPUT);
    pinMode(BUZZER, OUTPUT);

//Blynk init
    //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop()
{
    lcd.setCursor(1, 0);
    lcd.print("Enter Password");
    checkPass();
    rfidCheck();

    GASLevel();
    //Blynk.run();

    fireSensor();

    lightSensor();

    while (index_t == 1)
    {
        changePass();
    }

    while (index_t == 2)
    {
        resetPass();
    }

    while (index_t == 3)
    {
        openDoor();
        digitalWrite(LED, HIGH);
        delay(2000);
        digitalWrite(LED, LOW);
        time_error = 0;
    }

    while (index_t == 4)
    {
        errored3Times();
        //time_error = 0;
    }

    while(index_t == 5){
        errored5Times();
        time_error = 0;
    }

    while (index_t == 8)
    {
        addRFID();
    }

    while (index_t == 9)
    {
        delRFID();
    }

    while (index_t == 10)
    {
        delAllRFID();
    }
}