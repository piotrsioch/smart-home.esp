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
  {"reedSwitch", "/sensors/reed-switch"},
  {"dhtSensor", "/sensors/dht"},
  {"smokeSensor", "/sensors/smoke-sensor"},
  {"pirSensor", "/sensors/pir-sensor"},
  {"getLightState", "/light-state"},
};
