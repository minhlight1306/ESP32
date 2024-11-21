#ifndef SENSOR_H
#define SENSOR_H

#define BLYNK_TEMPLATE_ID "TMPL6qIHiFlyL"
#define BLYNK_TEMPLATE_NAME "Gas detector"
#define BLYNK_AUTH_TOKEN "6jPXx5C9nGfTjljoRc_pFUTT1gfjXGrS"

// #include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <WiFiClient.h>

//define Blynk
extern char auth[];
extern char ssid[];
extern char pass[];
//extern BlynkTimer timer;

//gas sensor
#define gas_sensor 34//A0
void GASLevel();

//fire sensor
#define fire_sensor 15//D0
//WidgetLED LED2(V2);
void fireSensor();

//sonic sensor
#define sonic_sensor 2//D0
void sonicSensor();

//light sensor
#define D0_light_sensor 33
#define A0_light_sensor 13
void lightSensor();

#endif // SENSOR_H