#include "sensor-functions.h"
#include "http-functions.h"
#include "globals.h"
#include <Arduino.h>
#include <DHT.h>

DHT dht(26, DHT11);

float previousTemperature = 0;
float previousHumidity = 0;

void initializeDHT() {
  dht.begin();
  delay(2000);
}

void IRAM_ATTR detectMovement() {
  digitalWrite(LED_PIN, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void handleReedSwitchState() {
    int actualState = digitalRead(REED_SWITCH);

    if (actualState != previousReedSwitchState) {
        std::string endpoint = endpointMap["reedSwitch"];
        std::string data = "";

        if (actualState == LOW) {
            data = "{\"isOpened\": false}";
        } else {
            data = "{\"isOpened\": true}";
        }

        sendRequest(endpoint, data);
        previousReedSwitchState = actualState;
    }
}

void handleDhtSensor() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if(temp != previousTemperature || humidity != previousHumidity) {
      std::string endpoint = endpointMap["dhtSensor"];
      String data = "{\"temperature\": \"" + String(temp) + "\", \"humidity\": \"" + String(humidity) + "\"}";
      
      previousTemperature = temp;
      previousHumidity = humidity;

      sendRequest(endpoint, data.c_str());
    }
}

void handleSmokeSensor() {
  int digitalNumber = analogRead(SMOKE_SENSOR_PIN); //from 0 to 4095

  if(digitalNumber > 150) {
    std::string endpoint = endpointMap["smokeSensor"];
    String data = "{\"digitalNumber\": \"" + String(digitalNumber) + "\"}";

    sendRequest(endpoint, data.c_str());
  }
}

void handlePirSensor() {
  std::string endpoint = endpointMap["pirSensor"];
  std::string data = "{\"movementDetected\": true}";

  sendRequest(endpoint, data);
}