#include "credentials.h"
#include "movement-detection.h"
#include "http-functions.h" 
#include "globals.h"

#include <map>
#include <string>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

DHT dht(26, DHT11);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  // DHT SETUP
  dht.begin();
  delay(2000);

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
  Serial.println(ssid);

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
  }
  
  if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
    digitalWrite(LED_PIN, LOW);
    startTimer = false;
    motion = false;
  }

  if(now - previousMillis >= INTERUPT_INTERVAL) {
    previousMillis = now;

    std::string endpointName = "reedSwitch";
    std::string data = "{\"isOpened\": true}";

    // sendRequestToEndpoint(endpointName, data);

    getAndSetLightState();

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int digitalNumber = analogRead(SMOKE_SENSOR_PIN); //from 0 to 4095

    int actualState = digitalRead(REED_SWITCH);

    if (actualState != previousState) {
      if (actualState == LOW) {
        Serial.println("CLOSED");
      } else {
        Serial.println("OPENED");
      }
      previousState = actualState;
    }

    // Serial.print("Temp: ");
    // Serial.print(temp);
    // Serial.println(" C ");
    // Serial.print("Humidity: ");
    // Serial.print(humidity);
    // Serial.println(" % ");
    
    // Serial.print("From smoke sensor: ");
    // Serial.println(digitalNumber);
  }
}
