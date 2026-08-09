//
// Created by Peasant on 8/4/2026.
//

#ifndef STM32TEMPCONTROL_STATUS_H
#define STM32TEMPCONTROL_STATUS_H
#include "main.h"

extern uint8_t g_target_temp ;

void StartStatusTask(void *argument);

void change_target_temp(uint8_t target_temp) ;

uint8_t read_target_temp() ;

#endif //STM32TEMPCONTROL_STATUS_H
