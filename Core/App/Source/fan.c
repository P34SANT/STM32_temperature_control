//
// Created by Peasant on 8/4/2026.
//

#include "../Include/fan.h"
#include "tim.h"
#include "cmsis_os.h"
#include "usart.h"

#define MAX_SPEED (99)
#define MIN_SPEED (0)
#define OFF        0
#define EMA_ALPHA 0.2f
#define MIN_TEMP 0
#define MAX_TEMP 100
#define MAX_DIFF 12
#define SPECIFIC_FAN_MIN 31

#define FAN_TASK_DELAY_MS 200

typedef enum {
    Direction_A,
    Direction_B
}Direction_t;

Direction_t Fan_Direction = Direction_A;

uint8_t speed = 0;

void Change_Fan_Speed(uint8_t speed);
void Set_Fan_Direction(Direction_t Direction);

void Fan_Init() {
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
    Change_Fan_Speed(OFF);
    //one of A or B must be LOW and other must be HIGH
    Set_Fan_Direction(Direction_A);
}

void Change_Fan_Speed(uint8_t target_speed)
{
    static float current_speed = 0.0f;

    if (target_speed > MAX_SPEED)
        target_speed = MAX_SPEED;

    current_speed += EMA_ALPHA * ((float)target_speed - current_speed);

    if (current_speed > MAX_SPEED)
        current_speed = MAX_SPEED;

    if (current_speed < MIN_SPEED)
        current_speed = MIN_SPEED;

    TIM5->CCR1 = (uint32_t)current_speed;
}

uint8_t calculate_speed(double current_temp, double target_temp)
{
    double diff = current_temp - target_temp;

    if (diff <= 0.0)
        return 0;

    if (diff >= MAX_DIFF)
        return MAX_SPEED;

    uint8_t tmpspd = (uint8_t)((diff / MAX_DIFF) * 100.0);

    if (tmpspd <= SPECIFIC_FAN_MIN)
        return SPECIFIC_FAN_MIN;

    return tmpspd;
}

void Set_Fan_Direction(Direction_t Direction) {
    if (Direction == Direction_A) {
        Fan_Direction = Direction_A;
        HAL_GPIO_WritePin(FAN_A_GPIO_Port, FAN_A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FAN_B_GPIO_Port, FAN_B_Pin, GPIO_PIN_RESET);
    }
    else {
        Fan_Direction = Direction_B;
        HAL_GPIO_WritePin(FAN_A_GPIO_Port, FAN_A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(FAN_B_GPIO_Port, FAN_B_Pin, GPIO_PIN_SET);
    }
}

uint8_t read_fan_speed() {
    return (uint8_t)TIM5->CCR1;
}

void StartFanTask(void *argument)
{
    /* Infinite loop */
    for(;;) {

        /*
         * Fan and heater must not run at the same time.
         */
        if (system_status.state == Warning &&
            system_status.heater_state == heater_off) {

            if (system_status.mode == AUTO) {

                speed = calculate_speed(
                    system_status.current_temp,
                    system_status.target_temp
                );

                Change_Fan_Speed(speed);

                if(speed > 0){
                    HAL_GPIO_WritePin(
                        RGB_BLUE_GPIO_Port,
                        RGB_BLUE_Pin,
                        GPIO_PIN_RESET
                    );
                }else {
                    HAL_GPIO_WritePin(
                        RGB_BLUE_GPIO_Port,
                        RGB_BLUE_Pin,
                        GPIO_PIN_SET
                    );
                }

            }else {
                Change_Fan_Speed(MIN_SPEED);

                HAL_GPIO_WritePin(
                    RGB_BLUE_GPIO_Port,
                    RGB_BLUE_Pin,
                    GPIO_PIN_SET
                );
            }

        }else {
            Change_Fan_Speed(MIN_SPEED);

            HAL_GPIO_WritePin(
                RGB_BLUE_GPIO_Port,
                RGB_BLUE_Pin,
                GPIO_PIN_SET
            );
        }

        osDelay(FAN_TASK_DELAY_MS);
    }
}