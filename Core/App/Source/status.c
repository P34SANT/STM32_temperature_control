//
// Created by Peasant on 8/4/2026.
//

#include "../App/Include/status.h"
#include "cmsis_os.h"
#include "../Include/sensor.h"

Status_t system_status = {
    .state = Init,
    .mode = AUTO,
    .target_temp = 0,
    .current_temp = 0,
    .fan_speed = 0,
    .heater_state = heater_off,
};


void Change_State(State_t new_state) {
    system_status.state = new_state;
}

void change_target_temp(const uint8_t target_temp) {
    system_status.target_temp = target_temp;
}

uint8_t read_target_temp() {
    return system_status.target_temp;

}

void status_monitor () {
    system_status.current_temp = temp_sensor_value;
    system_status.fan_speed = 0; //must be updated
    system_status.heater_state = 0;// must be updated
}

void change_to_manual_mode() {
    system_status.mode = MANUAL;
}

void change_to_auto_mode() {
    system_status.mode = AUTO;
}
Status_t get_system_status() {
    return system_status;
}

void StartStatusTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        status_monitor();
        osDelay(200);
    }

}