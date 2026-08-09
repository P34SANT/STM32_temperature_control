//
// Created by Peasant on 8/4/2026.
//

#include "../Include/lcd.h"
#include "../Library/ALCD.h"
#include "cmsis_os.h"

void LCD_Init(void) {
    ALCD_init(16 , 2);
    ALCD_clear();
}

void LCD_Clear(void) {
    ALCD_clear();
}

void StartLcdTask(void *argument)
{

    /* Infinite loop */
    // ReSharper disable once CppDFAEndlessLoop
    for(;;)
    {

        osDelay(1);
    }

}

