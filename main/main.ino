#include "credentials.h"
#include "sensor-functions.h"
#include "http-functions.h" 
#include "globals.h"

#include <map>
#include <string>
#include <WiFi.h>
#include <HTTPClient.h>

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // DHT SETUP
  initializeDHT();

  // MOTION SENSOR SETUP
  pinMode(MOTION_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTION_SENSOR_PIN), detectMovement, RISING);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // REED SWITCH SETUP
  pinMode(REED_SWITCH, INPUT_PULLUP);

  // LIGHT SETUP
  pinMode(LIGHT_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting with wifi.");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected with wifi!");
}

void loop() {
  now = millis();

  if((digitalRead(LED_PIN) == HIGH) && (motion == false)) {
    motion = true;
    handlePirSensor();
  }
  
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    digitalWrite(LED_PIN, LOW);
    startTimer = false;
    motion = false;
  }

  if(now - previousMillis >= INTERUPT_INTERVAL) {
    previousMillis = now;

    getAndSetLightState();
    handleReedSwitchState();
    handleDhtSensor();
    handleSmokeSensor();
  }
}
