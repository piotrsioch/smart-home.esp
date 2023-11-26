#include <DHT.h>
#include <WiFi.h>
#include "credentials.h"
#include <HTTPClient.h>
#include <map>
#include <string>

#define timeSeconds 2
#define LED_PIN 16
#define MOTION_SENSOR_PIN 17
#define SMOKE_SENSOR_PIN 32
#define REED_SWITCH 22

std::map<std::string, std::string> endpointMap = {
  {"reedSwitch", "/sensor/reed-switch"},
  {"anotherEndpoint", "/another/path"}
};

DHT dht(26, DHT11);

WiFiClient client;

const long INTERUPT_INTERVAL = 2000;

unsigned long now = millis();
unsigned long previousMillis = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

bool previousState = HIGH;


void IRAM_ATTR detectMovement() {
  digitalWrite(LED_PIN, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

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

    sendRequestToEndpoint(endpointName, data);

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

void sendRequestToEndpoint(const std::string& endpointName, const std::string& data) {
  if (endpointMap.find(endpointName) != endpointMap.end()) {
    std::string path = endpointMap[endpointName];

    sendRequest(path, data);
  } else {
    Serial.println("There is no endpoint with given name");
  }
}

void sendRequest(const std::string& path, const std::string& data) {
  HTTPClient http;

  String url = "http://" + String(serverIP) + ":" + String(serverPort) + String(path.c_str());
  Serial.println(url);

  http.begin(url.c_str());
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data.c_str());

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.println("Error occured while sending data to server");
  }

  http.end();
}