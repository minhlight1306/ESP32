#include <sensor.h>
#include <lock_door.h>

char auth[] = "6jPXx5C9nGfTjljoRc_pFUTT1gfjXGrS";
char ssid[] = "Wi_MESH 2.4G";
char pass[] = "";

//fire init
int clap = 0;
long detection_range_start = 0;
long detection_range = 0;
boolean status_lights = false;

// WidgetLED LED1(V1);//gas

// WidgetLED LED2(V2);//fire

//gas function
void GASLevel() {
  int value = analogRead(gas_sensor);
  value = map(value, 0, 4095, 0, 100);
  //Serial.println(value);
  //Blynk.virtualWrite(V0, value);

  if (value >= 50) {
    digitalWrite(LED, HIGH);
    delay(1000);
    for(int i = 0; i < 5; i++) //gas keu 5s
      digitalWrite(BUZZER, HIGH);
    //LED1.on();
  } else {
    digitalWrite(LED, LOW);
    //LED1.off();
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
    //LED2.off();
  }

  else {
    //Serial.println("Flame dected => The fire is detected");
    digitalWrite(LED, LOW);
    //LED2.on();
  }
}

//sonic function
void sonicSensor(){
  int status_sensor = digitalRead(sonic_sensor);
  if(status_sensor == 0){
    if(clap == 0){
      detection_range_start = detection_range = millis();
      clap++;
    }
    else if(clap > 0 && millis() - detection_range >= 50){
      detection_range = millis();
      clap++;
    }
  }
  if(millis() - detection_range_start >= 400){
    if(clap == 2){
      if(!status_lights){
        status_lights = true;
        digitalWrite(LED, HIGH);
        // delay(2000);
      }
      else if(status_lights){
        status_lights = false;
        digitalWrite(LED, LOW);
      }
    }
    clap = 0;
  }
}

//light function
void lightSensor(){
  // int lightValue = analogRead(A0_light_sensor);
  // int lightState = digitalRead(D0_light_sensor);

  // Serial.print("The A0 value: ");
  // Serial.println(lightValue);
  // if(lightState == HIGH){
  //   Serial.println("It is dark");
  //   digitalWrite(LED, HIGH);
  // }
  // else{
  //   Serial.println("It is light");
  //   digitalWrite(LED, LOW);
  // }
}