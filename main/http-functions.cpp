#include "http-functions.h"
#include "globals.h"
#include "credentials.h"
#include <WiFi.h> 
#include <map>
#include <Arduino.h>

std::map<std::string, std::string> endpointMap = {
  {"reedSwitch", "/sensor/reed-switch"},
  {"getLightState", "/light-state"}
};

bool ledState = false;

void sendRequest(const std::string& path, const std::string& data) {
  HTTPClient http;

  String url = "http://" + String(serverIP) + ":" + String(serverPort) + String(path.c_str());

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
            Serial.println(response);

            int index = response.indexOf("lightState");
            if (index != -1) {
                int startIndex = response.indexOf(":", index) + 2;
                int endIndex = response.indexOf("\"", startIndex);

                if (startIndex != -1 && endIndex != -1) {
                    String state = response.substring(startIndex, endIndex);
                    Serial.println(state);
                    if (state == "off") {
                      Serial.println("In off");
                      ledState = false;
                      digitalWrite(LIGHT_PIN, LOW);
                    } else if (state == "on") {
                      Serial.println("In on");
                      ledState = true;
                      digitalWrite(LIGHT_PIN, HIGH);
                    }
                }
            }
        } else {
            Serial.println("Error during HTTP request");
        }

        http.end();
    }
}
