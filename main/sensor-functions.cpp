#include "sensor-functions.h"
#include "http-functions.h"
#include "globals.h"
#include <Arduino.h>
#include "credentials.h"
#include <DHT.h>

DHT dht(26, DHT11);

float previousTemperature = 0;
float previousHumidity = 0;

void initializeSensors() {
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
}

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
    String data = "";

    if (actualState == LOW) {
      data = "{\"sensorId\": \"" + reedSwitchId + "\", \"isOpened\": false}";
    } else {
      data = "{\"sensorId\": \"" + reedSwitchId + "\", \"isOpened\": true}";
    }

    sendRequest(endpoint, data.c_str());
    previousReedSwitchState = actualState;
  }
}

void handleDhtSensor() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if(temp != previousTemperature || humidity != previousHumidity) {
      std::string endpoint = endpointMap["dhtSensor"];
      String data = "{\"sensorId\": \"" +  dhtSensorId + "\", \"temperature\": \"" + String(temp) + "\", \"humidity\": \"" + String(humidity) + "\"}";
      
      previousTemperature = temp;
      previousHumidity = humidity;

      sendRequest(endpoint, data.c_str());
    }
}

void handleSmokeSensor() {
  int digitalNumber = analogRead(SMOKE_SENSOR_PIN); //from 0 to 4095

  if(digitalNumber > 150) {
    std::string endpoint = endpointMap["smokeSensor"];
    String data = "{\"sensorId\": \"" +  smokeSensorId + "\", \"value\": \"" + String(digitalNumber) + "\"}";

    sendRequest(endpoint, data.c_str());
  }
}

void handlePirSensor() {
  std::string endpoint = endpointMap["pirSensor"];

  String data = "{\"sensorId\": \"" + pirSensorId  + "\"}";

  sendRequest(endpoint, data.c_str());
}

void handleSensors() {
  getAndSetLightState();
  handleReedSwitchState();
  handleDhtSensor();
  handleSmokeSensor();
}