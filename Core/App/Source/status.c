//
// Created by Peasant on 8/4/2026.
//

#include "../App/Include/status.h"
#include "cmsis_os.h"

uint8_t g_target_temp = 0;

void StartStatusTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }

}

void change_target_temp(uint8_t target_temp) {
    g_target_temp = target_temp;
}

uint8_t read_target_temp() {
    return g_target_temp;

}

