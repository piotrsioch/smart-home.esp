#include "movement-detection.h"
#include "globals.h"
#include <Arduino.h>

void IRAM_ATTR detectMovement() {
   digitalWrite(LED_PIN, HIGH);
  startTimer = true;
  lastTrigger = millis();
}