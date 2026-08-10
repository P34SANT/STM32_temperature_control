//
// Created by Peasant on 8/4/2026.
//

#ifndef STM32TEMPCONTROL_HEATER_H
#define STM32TEMPCONTROL_HEATER_H
#include "main.h"

void heater_ON();
void heater_OFF();

void StartHeaterTask(void *argument);

#endif //STM32TEMPCONTROL_HEATER_H
