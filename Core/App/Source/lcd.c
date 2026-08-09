//
// Created by Peasant on 8/4/2026.
//

#include "../Include/lcd.h"
#include "../Library/ALCD.h"
#include "cmsis_os.h"
#define LCD_COLUMN 16
#define LCD_ROW    2
#define LCD_UPDATE_INTERVAL_MS 200

void LCD_Init(void) {
    ALCD_init(LCD_COLUMN , LCD_ROW);
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
        char * mode = "";
        char * state = "";
        if (system_status.mode == MANUAL) {
            mode = "Manual";
        }else {
            mode = "Auto";
        }
        switch (system_status.state) {
            case Init:
                state = "Init";
                break;
            case Normal :
                state = "Normal";
                break;
            case Warning:
                state = "Warning";
                break;
            case Error:
                state = "Error";
                break;
        }
        ALCD_printfXY(0 , 0 , "Trgt:%02d  Crnt:%02d" , system_status.target_temp , system_status.current_temp);
        ALCD_printfXY(0 , 1 , state);
        ALCD_printfXY(9 , 1 , mode);

        osDelay(pdMS_TO_TICKS(LCD_UPDATE_INTERVAL_MS));
    }

}

