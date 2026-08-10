//
// Created by Peasant on 8/4/2026.
//

#include "../App/Include/status.h"

#include <string.h>

#include "cmsis_os.h"
#include "gpio.h"
#include "iwdg.h"
#include "../Include/sensor.h"
#include "../App/Include/fan.h"
#define STATUS_TASK_DELAY_MS 200
#define SOFTWARE_BTN_DEBOUNCE_MS 0

Status_t system_status = {
    .state = Init,
    .mode = AUTO,
    .target_temp = 10,
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
    system_status.fan_speed = read_fan_speed();
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

void check_buttons(){
    if (BTN_FLAG.BTN_MODE == 1) {
            memset(&BTN_FLAG, 0, sizeof(BTN_FLAG));
            if (system_status.mode == MANUAL) {
                change_to_auto_mode();

            }else{change_to_manual_mode();}


    }
    if (BTN_FLAG.BTN_T_UP == 1) {

            memset(&BTN_FLAG, 0, sizeof(BTN_FLAG));
            system_status.target_temp ++;

    }
    if (BTN_FLAG.BTN_T_DOWN == 1) {

            memset(&BTN_FLAG, 0, sizeof(BTN_FLAG));
            system_status.target_temp --;

    }

}

void StartStatusTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        if(system_status.current_temp != system_status.target_temp) {
            system_status.state = Warning;
        }else {
            system_status.state = Normal;
        }
        status_monitor();

        HAL_IWDG_Refresh(&hiwdg);

        osDelay(STATUS_TASK_DELAY_MS);
    }

}

