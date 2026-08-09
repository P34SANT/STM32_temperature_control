//
// Created by Peasant on 8/4/2026.
//

#ifndef STM32TEMPCONTROL_STATUS_H
#define STM32TEMPCONTROL_STATUS_H
#include "main.h"

typedef enum {
    Init,
    Normal,
    Warning,
    Error
  }State_t;

extern State_t g_state;

typedef enum {
    AUTO,
    MANUAL
}Mode_t;

typedef enum {
    heater_on,
    heater_off,
}heater_state_t;

typedef struct {
    State_t state;
    Mode_t mode;
    uint8_t target_temp;
    int16_t current_temp;
    uint8_t fan_speed;
    heater_state_t heater_state;
}Status_t;

extern Status_t system_status ;

void Change_State(State_t new_state) ;

void StartStatusTask(void *argument);

void change_target_temp(uint8_t target_temp) ;

void change_to_manual_mode() ;

void change_to_auto_mode() ;

Status_t get_system_status() ;

uint8_t read_target_temp() ;

#endif //STM32TEMPCONTROL_STATUS_H
