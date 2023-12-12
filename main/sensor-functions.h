#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

#include <Arduino.h>

void IRAM_ATTR detectMovement();
void initializeSensors();
void handleSensors();
void handleReedSwitchState();
void initializeDHT();
void handleDhtSensor();
void handleSmokeSensor();
void handlePirSensor();
void turnOnAlarm();
void handleChangeAlarmState();

#endif
