//
// Created by Peasant on 8/4/2026.
//

#include "../Include/fan.h"
#include "tim.h"
#include "cmsis_os.h"

#define MAX_SPEED (100-1)
#define MIN_SPEED (0)
#define OFF        0

typedef enum {
    Direction_A,
    Direction_B
}Direction_t;

Direction_t Fan_Direction = Direction_A;


void Change_Fan_Speed(uint8_t speed) ;
void Set_Fan_Direction(Direction_t Direction);

void Fan_Init() {
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    Change_Fan_Speed(OFF);
    //one of A or B must be LOW and other must be HIGH
    Set_Fan_Direction(Direction_A);
}

void Change_Fan_Speed(uint8_t speed) {
    if (speed < MAX_SPEED) {
        TIM5->CCR1 = speed;
    }else{//err handling
        TIM5->CCR1 = MAX_SPEED;
    }
}

void Set_Fan_Direction(Direction_t Direction) {
    if (Direction == Direction_A) {
        Fan_Direction = Direction_A;
        HAL_GPIO_WritePin(FAN_A_GPIO_Port , FAN_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FAN_B_GPIO_Port , FAN_B_Pin, GPIO_PIN_RESET);
    }
    else {
        Fan_Direction = Direction_B;
        HAL_GPIO_WritePin(FAN_A_GPIO_Port , FAN_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FAN_B_GPIO_Port , FAN_B_Pin, GPIO_PIN_SET);
    }
}

void StartFanTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        osDelay(1);
    }

}
