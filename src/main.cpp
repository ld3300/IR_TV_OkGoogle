#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "private.h"

#define SLAVEADDR 8               // Address of I2C Slave

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe tvcontrol = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/tvcontrol");

void MQTT_connect() {
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected()) return;

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {   // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0){
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

void setup(){
  Serial.begin(115200);
  Wire.begin();

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println("OK!");
  mqtt.subscribe(&tvcontrol);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  //Connect/Reconnect to MQTT
  MQTT_connect();

  // Read from our subscription queue until we run out, or
  // wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000))){
    //If we're in here, a subscription updated...
    if (subscription == &tvcontrol){
      char buffer[50];
      sprintf(buffer, "esp%s", (char*)tvcontrol.lastread); 
      Wire.beginTransmission(SLAVEADDR);
      Wire.write(buffer);
      Wire.endTransmission();                    

      //Print the new value to the serial monitor
      Serial.print("tvcontrol: ");
      Serial.println(buffer);
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) tvcontrol.lastread, "on")){
        //active low logic
        digitalWrite(LED_BUILTIN, LOW);
      }
      else if (!strcmp((char*) tvcontrol.lastread, "on")){
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping()){
    mqtt.disconnect();
  }

 
  // Wire.beginTransmission(SLAVEADDR);
  // Wire.write("test");
  // int _error = Wire.endTransmission();
  // Serial.println("_error = ");
  // Serial.println(_error);
  // delay(500);
}