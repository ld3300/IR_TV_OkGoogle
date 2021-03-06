/***************************************************
  Using Adafruit MQTT Library

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/* IRremoteESP8266
 * Version 1.0 April, 2017
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#define FW_VERSION "0.1-beta"

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <IRLibSendBase.h>
#include <IRLibDecodeBase.h>
#include <IRLib_P01_NEC.h> 
#include <IRLib_P02_Sony.h> 
#include <IRLibCombo.h>
#include <IRLibRecv.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "private.h"
#include "VIZIO_IRcodes.h"

#define TV_PROTOCOL NECX                // Vizio TV Remote Protocol
#define TV_BITS 32
#define TV_INPUT 0x20DF639C             // Vizio tv switch inputs
#define REMOTE_PROTOCOL SONY
#define REMOTE_BIT 20
#define REMOTE_INPUT 682823

IRrecv myReceiver(14);   // Pin to receive IR
// const uint16_t kIrLed = 12;      // Pin to send IR

IRsend mySender;          // might be pin 3
IRdecode myDecoder;
WiFiClient client;

// const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
// const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
// const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;
// const uint32_t MQTT_PORT PROGMEM    = AIO_PORT;

Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe tvcontrol = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/tvcontrol");
Adafruit_MQTT_Subscribe tvDebug = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/feeds/tvdebug");

void CheckInput(uint8_t IR_protocol, uint32_t IR_value, uint16_t IR_bits){    // Check if Input command received
  if(IR_protocol == REMOTE_PROTOCOL && IR_value == REMOTE_INPUT && IR_bits == REMOTE_BIT){       // Received signal from dish remote
    mySender.send(TV_PROTOCOL,TV_INPUT,TV_BITS);
    myReceiver.enableIRIn();    // Make sure receiver doesn't get stopped by sender
  }
}

void CheckIR(){                                       // Decode results from IR remote
  if(myReceiver.getResults()) { 
    if(myDecoder.decode()) {
      myDecoder.dumpResults(false); // print results
      uint8_t IR_protocol = myDecoder.protocolNum;
      uint32_t IR_value = myDecoder.value;
      uint16_t IR_bits = myDecoder.bits;
      CheckInput(IR_protocol, IR_value, IR_bits);   // Check if received signal for switch TV inputs
    }
    myReceiver.enableIRIn();      // Restart receiver
  }
}

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

void OTAInit(){
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(ESP_HOSTNAME);

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void setup(){
  Serial.begin(115200);
  Serial.println(FW_VERSION);
  myReceiver.enableIRIn();
  Serial.println(F("Ready to receive IR signals"));

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  OTAInit();

  mqtt.subscribe(&tvcontrol);
  mqtt.subscribe(&tvDebug);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(){
  ArduinoOTA.handle();
  yield();
  //Connect/Reconnect to MQTT
  MQTT_connect();

  // Read from our subscription queue until we run out, or
  // wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000))){
    //If we're in here, a subscription updated...
    if (subscription == &tvcontrol){
      char buffer[50];
      sprintf(buffer, "%s", (char*)tvcontrol.lastread); 

      //Print the new value to the serial monitor
      Serial.print("tvcontrol: ");
      Serial.println(buffer);
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (strstr(buffer, "on") != NULL){
        //active low logic
        digitalWrite(LED_BUILTIN, LOW);
        sendIR(POWERON);
        Serial.println("On executed");
      }
      else if (strstr(buffer, "off") != NULL){
        digitalWrite(LED_BUILTIN, HIGH);
        sendIR(POWEROFF);
        Serial.println("Off executed");
      }
      else if (strstr(buffer, "HDMI 1") != NULL){
        sendIR(HDMI1);
        Serial.println("HDMI1 executed");
      }
      else if (strstr(buffer, "HDMI 2") != NULL){
        sendIR(HDMI2);
        Serial.println("HDMI2 executed");
      }
      else if (strstr(buffer, "HDMI 3") != NULL){
        sendIR(HDMI3);
        Serial.println("HDMI3 executed");
      }
      else if (strstr(buffer, "HDMI 4") != NULL){
        sendIR(HDMI4);
        Serial.println("HDMI4 executed");
      }
      else if (strstr(buffer, "mute") != NULL){
        sendIR(MUTE);
        Serial.println("MUTE executed");
      }
      else if (strstr(buffer, "unmute") != NULL){
        sendIR(MUTE);
        Serial.println("UNMUTE executed");
      }
      else if (strstr(buffer, "on mute") != NULL){
        sendIR(MUTE);
        Serial.println("MUTE executed");
      }
    }
    else if (subscription == &tvDebug){
      char buffer[50];
      sprintf(buffer, "%s", (char*)tvDebug.lastread); 

      //Print the new value to the serial monitor
      Serial.print("tvDebug: ");
      Serial.println(buffer);
      unsigned int i;
      sscanf(buffer, "%u", &i);
      Serial.println(i, HEX);
      sendIR(i);
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping()){
    mqtt.disconnect();
  }

  CheckIR();
}