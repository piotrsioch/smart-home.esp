#include "globals.h"
#include <Arduino.h>

const long INTERUPT_INTERVAL = 2000;
unsigned long now = millis();
unsigned long previousMillis = millis();
unsigned long lastTrigger = 0;
bool startTimer = false;
bool motion = false;
bool previousState = HIGH;
