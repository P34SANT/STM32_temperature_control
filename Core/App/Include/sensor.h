//
// Created by Peasant on 8/4/2026.
//

#ifndef STM32TEMPCONTROL_SENSOR_H
#define STM32TEMPCONTROL_SENSOR_H
#include "main.h"

void Sensor_Init();

extern volatile double temp_sensor_value ;

void StartSensorTask(void *argument);

#endif //STM32TEMPCONTROL_SENSOR_H
