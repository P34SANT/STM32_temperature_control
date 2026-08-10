//
// Created by Peasant on 8/4/2026.
//

#ifndef STM32TEMPCONTROL_FAN_H
#define STM32TEMPCONTROL_FAN_H
#include "main.h"


void Fan_Init() ;
void StartFanTask(void *argument);
uint8_t read_fan_speed() ;

#endif //STM32TEMPCONTROL_FAN_H
