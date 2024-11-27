#include <Arduino.h>

//define blynk
#define BLYNK_TEMPLATE_ID "TMPL6qIHiFlyL"
#define BLYNK_TEMPLATE_NAME "Gas detector"
#define BLYNK_AUTH_TOKEN "6jPXx5C9nGfTjljoRc_pFUTT1gfjXGrS"

//gas sensor
#define gas_sensor 34//A0

//fire sensor
#define fire_sensor 15//D0

//sonic sensor
#define sonic_sensor 2//D0

//light sensor
#define D0_light_sensor 33
#define A0_light_sensor 13

//temperature sensor
#define data_temperature_sensor 4
#define DHTTYPE DHT11

//include libraries
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <lock_door.h>

//blynk variable
char auth[] = "6jPXx5C9nGfTjljoRc_pFUTT1gfjXGrS";
char ssid[] = "ACLAB";
char pass[] = "";
WidgetLED LED1(V1); //gas
WidgetLED LED2(V2); //fire

//sonic variable
int clap = 0;
long detection_range_start = 0;
long detection_range = 0;
boolean status_lights = false;

//temperature variable
DHT dht11(data_temperature_sensor, DHTTYPE);

//gas function
void GASLevel() {
  int value = analogRead(gas_sensor);
  value = map(value, 0, 4095, 0, 100);
  //Serial.println(value);
  Blynk.virtualWrite(V0, value);

  if (value >= 50) {
    digitalWrite(LED, HIGH);
    delay(1000);
    for(int i = 0; i < 5; i++) //gas keu 5s
      digitalWrite(BUZZER, HIGH);
    LED1.on();
  } else {
    // digitalWrite(LED, LOW);
    LED1.off();
  }
}

//fire function
void fireSensor(){
  int flame_state = digitalRead(fire_sensor);

  if (flame_state == HIGH) {
    //Serial.println("No flame dected => The fire is NOT detected");
    digitalWrite(LED, HIGH);
    for(int i = 0; i < 7; i++) //fire keu 7s
      digitalWrite(BUZZER, HIGH);
    delay(2000);
    LED2.off();
  }

  else {
    //Serial.println("Flame dected => The fire is detected");
    // digitalWrite(LED, LOW);
    LED2.on();
  }
}

//sonic function---------not used
// void sonicSensor(){
//   int status_sensor = digitalRead(sonic_sensor);
//   if(status_sensor == 0){
//     if(clap == 0){
//       detection_range_start = detection_range = millis();
//       clap++;
//     }
//     else if(clap > 0 && millis() - detection_range >= 50){
//       detection_range = millis();
//       clap++;
//     }
//   }
//   if(millis() - detection_range_start >= 400){
//     if(clap == 2){
//       if(!status_lights){
//         status_lights = true;
//         digitalWrite(LED, HIGH);
//         delay(2000);
//       }
//       else if(status_lights){
//         status_lights = false;
//         digitalWrite(LED, LOW);
//       }
//     }
//     clap = 0;
//   }
// }

//light function
void lightSensor(){
  // int lightValue = analogRead(A0_light_sensor);
  int lightState = digitalRead(D0_light_sensor);

  // Serial.print("The A0 value: ");
  // Serial.println(lightValue);
  if(lightState == HIGH){
    Serial.println("It is dark");
    digitalWrite(LED, HIGH);
  }
  else{
    Serial.println("It is light");
    // digitalWrite(LED, LOW);
  }
}

//temperature function
void temperatureSensor(){
  float Celsius_degree = dht11.readTemperature();
  float humidity = dht11.readHumidity();
  if(isnan(Celsius_degree) || isnan(humidity)){
    Serial.println("Fail to read temperature");
    return;
  }
  //Compute heat index in Celsius (isFahreheit = false)
  float hi = dht11.computeHeatIndex(Celsius_degree, humidity, false);
  Serial.printf("Temperature: %f°C\n", Celsius_degree);
  Serial.printf("Heat index: %f°C\n", hi);
  delay(2000);
}

//lock door function
void lock_door(){
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

void setup()
{
    Serial.begin(9600);
    EEPROM.begin(512);
//init servo
    sg90.setPeriodHertz(50);
    sg90.attach(PIN_SG90, 500, 2400);
//init SPI
    SPI.begin();
//init RFID
    rfid.PCD_Init();
//init temperature sensor
    dht11.begin();
//init lcd to print
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
    pinMode(sonic_sensor, INPUT);
    pinMode(data_temperature_sensor, INPUT);
    pinMode(BUZZER, OUTPUT);
//   lcd.print("begin");
// //Blynk init
//     Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
//     lcd.print("end");
}

void loop()
{
    lcd.setCursor(1, 0);
    lcd.print("Enter Password");
    checkPass();
    rfidCheck();

    Blynk.run();
    GASLevel();
    temperatureSensor();
    fireSensor();
    lightSensor();
    // sonicSensor(); // not used

    lock_door();
}