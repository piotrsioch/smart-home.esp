#include "http-functions.h"
#include "globals.h"
#include "credentials.h"
#include <WiFi.h> 
#include <map>

std::map<std::string, std::string> endpointMap = {
  {"reedSwitch", "/sensor/reed-switch"},
  {"anotherEndpoint", "/another/path"}
};

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
