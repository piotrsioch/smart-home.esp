#include "credentials.h"
#include "sensor-functions.h"
#include "http-functions.h" 
#include "globals.h"

#include <map>
#include <string>
#include <WiFi.h>
#include <HTTPClient.h>

void setup() {
  Serial.begin(115200);

  initializeSensors();
  initializeWifi();
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

    handleSensors();
  }
}
