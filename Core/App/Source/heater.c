//
// Created by Peasant on 8/4/2026.
//

#include "../Include/heater.h"
#include "cmsis_os.h"

#define HYSTERSIS_C 1

heater_state_t old_state = heater_off;

void heater_ON() {
    HAL_GPIO_WritePin(
        RGB_GREEN_GPIO_Port,
        RGB_GREEN_Pin,
        GPIO_PIN_RESET
    );
}

void heater_OFF() {
    HAL_GPIO_WritePin(
        RGB_GREEN_GPIO_Port,
        RGB_GREEN_Pin,
        GPIO_PIN_SET
    );
}


void StartHeaterTask(void *argument)
{
    /* Infinite loop */
    for(;;)
    {
        if (system_status.mode == AUTO) {

            if ((float)system_status.current_temp <
                (float)system_status.target_temp - HYSTERSIS_C) {

                if (old_state != heater_on) {
                    system_status.heater_state = heater_on;
                    heater_ON();
                    old_state = heater_on;
                }

                }else if ((float)system_status.current_temp >
                          (float)system_status.target_temp + HYSTERSIS_C) {

                    if (old_state != heater_off) {
                        system_status.heater_state = heater_off;
                        heater_OFF();
                        old_state = heater_off;
                    }
                          }

        }else if (system_status.mode == MANUAL) {

            heater_OFF();
            system_status.heater_state = heater_off;
            old_state = heater_off;

        }

        osDelay(10);
    }
}