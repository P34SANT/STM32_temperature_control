//
// Created by Peasant on 8/4/2026.
//

#include "../Include/heater.h"
#include "cmsis_os.h"

void StartHeaterTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }

}