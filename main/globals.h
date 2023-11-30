#ifndef GLOBALS_H
#define GLOBALS_H

#include <map>
#include <string>

#define timeSeconds 5
#define LIGHT_PIN 14
#define LED_PIN 16
#define MOTION_SENSOR_PIN 17
#define SMOKE_SENSOR_PIN 32
#define REED_SWITCH 22

extern const long INTERUPT_INTERVAL;
extern unsigned long now;
extern unsigned long previousMillis;
extern unsigned long lastTrigger;
extern bool startTimer;
extern bool motion;
extern bool previousReedSwitchState;
extern std::map<std::string, std::string> endpointMap;

#endif
