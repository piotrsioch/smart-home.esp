#include "http-functions.h"
#include "globals.h"
#include "credentials.h"
#include <WiFi.h> 
#include <map>
#include <Arduino.h>

WiFiClient client;

bool ledState = false;

void initializeWifi() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting with wifi.");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected with wifi!");
}

void sendRequest(const std::string& path, const std::string& data) {
  HTTPClient http;

  String url = "http://" + String(serverIP) + ":" + String(serverPort) + String(path.c_str());

  http.begin(url.c_str());
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(data.c_str());

  if (httpResponseCode > 0) {
    String response = http.getString();
  } else {
    Serial.println("Error occured while sending data to server");
  }

  http.end();
}

void sendRequestToEndpoint(const std::string& endpointName, const std::string& data) {
  if (endpointMap.find(endpointName) != endpointMap.end()) {
    std::string path = endpointMap[endpointName];

    sendRequest(path, data);
  } else {
    Serial.println("There is no endpoint with given name");
  }
}

void getAndSetLightState() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    std::string endpoint = endpointMap["getLightState"];
  
    String url = "http://" + String(serverIP) + ":" + String(serverPort) + String(endpoint.c_str());

    http.begin(url);

    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();

      int index = response.indexOf("isOn");
      if (index != -1) {
        int startIndex = response.indexOf(":", index) + 2;
        int endIndex = response.indexOf("\"", startIndex);

          if (startIndex != -1 && endIndex != -1) {
            String state = response.substring(startIndex, endIndex);

            if ((state == "false" && ledState) || (state == "true" && !ledState)) {
              ledState = !ledState;
              digitalWrite(LIGHT_PIN, ledState ? HIGH : LOW);
            }
          }
        }
    } else {
      Serial.println("Error during HTTP request");
    }

    http.end();
  }
}

void getAndSetAlarmState() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    std::string endpoint = endpointMap["getAlarmState"];

    String url = "http://" + String(serverIP) + ":" + String(serverPort) + String(endpoint.c_str());

    http.begin(url);

    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();

      int index = response.indexOf("state");
      if (index != -1) {
        int startIndex = response.indexOf(":", index) + 2;
        int endIndex = response.indexOf("\"", startIndex);

        if (startIndex != -1 && endIndex != -1) {
          alarmState = response.substring(startIndex, endIndex);
        }
      }
    } else {
      Serial.println("Error during HTTP request");
    }

    http.end();
  }
}

