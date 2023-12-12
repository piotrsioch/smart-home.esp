#include "globals.h"
#include <Arduino.h>

const long INTERUPT_INTERVAL = 2000;
unsigned long now = millis();
unsigned long previousMillis = millis();
unsigned long lastTrigger = 0;
bool startTimer = false;
bool motion = false;
bool previousReedSwitchState = HIGH;
std::map<std::string, std::string> endpointMap = {
  {"reedSwitch", "/reed-switch/add-data"},
  {"dhtSensor", "/dht-sensor/add-data"},
  {"smokeSensor", "/smoke-sensor/add-data"},
  {"pirSensor", "/pir-sensor/add-data"},
  {"getLightState", "/light/get-state"},
  {"getAlarmState", "/alarm/get-state"},
};
String alarmState = "";
