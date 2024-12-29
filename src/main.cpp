#include <Arduino.h>

//define blynk
#define BLYNK_TEMPLATE_ID "TMPL6sA5MV9y7"
#define BLYNK_TEMPLATE_NAME "Sensors"
#define BLYNK_AUTH_TOKEN "XX_7Zb8WemzjEIvewp-8nQi_qMWL1Z6l"

//gas sensor
#define gas_sensor 34//A0

//fire sensor
#define fire_sensor 15//D0

//temperature sensor
#define data_temperature_sensor 4
#define DHTTYPE DHT11
#define LED_2 2

//include libraries
// #include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <lock_door.h>

//blynk variable
// char auth[] = "6jPXx5C9nGfTjljoRc_pFUTT1gfjXGrS";
// char ssid[] = "";
// char pass[] = "";
// WidgetLED LED1(V1); //gas
// WidgetLED LED2(V2); //fire

//temperature variable
DHT dht(data_temperature_sensor, DHTTYPE);

//gas function
void GASLevel() {
  int value = analogRead(gas_sensor);
  value = map(value, 0, 4095, 0, 100);
  //Serial.println(value);
  // Blynk.virtualWrite(V0, value);

  if (value >= 50) {
    digitalWrite(LED, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    // LED1.on();
  } else {
    digitalWrite(LED, LOW);
    digitalWrite(BUZZER, LOW);
    // LED1.off();
  }
}

//fire function
void fireSensor(){
  int flame_state = digitalRead(fire_sensor);

  if (flame_state == HIGH) {
    //Serial.println("No flame dected => The fire is NOT detected");
    digitalWrite(LED, HIGH);
      digitalWrite(BUZZER, HIGH);
    // delay(2000);
    // LED2.off();
  }

  else {
    //Serial.println("Flame dected => The fire is detected");
    // digitalWrite(LED, LOW);
    // LED2.on();
  }
}


//temperature function
void temperatureSensor(){
  float Celsius_degree = dht.readTemperature();
  if(isnan(Celsius_degree)){
    // Serial.println("Fail to read temperature");
    // return;
  }
  if(Celsius_degree > 30){
    digitalWrite(LED_2, HIGH);
  }
  //Compute heat index in Celsius (isFahreheit = false)
  // Serial.printf("Temperature: %f°C\n", Celsius_degree);
  
  // delay(2000);
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
        time_error = 0;
    }

    if (index_t == 4)
    {
        errored3Times();
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
    dht.begin();
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
    pinMode(data_temperature_sensor, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(LED_2, OUTPUT);
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
    lock_door();

    // Blynk.run();
    GASLevel();
    temperatureSensor();
    fireSensor();


}