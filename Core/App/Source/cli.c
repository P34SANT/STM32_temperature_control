//
// Created by Peasant on 8/4/2026.
//

#include "../Include/cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "../Library/DMAbuffer.h"
#include "usart.h"

#define CLI_BUFFER_SIZE 256
#define CLI_PROCESS_BUFFER_SIZE 52

/*Commands:
 *$SET_TEMP
 *$MODE=AUTO/MANUAL
 *$STATUS
 */
#define CMD_MODE "$MODE="
#define CMD_MODE_MANUAL "MANUAL"
#define CMD_MODE_AUTO "AUTO"
#define CMD_TEMP "$SET_TEMP"
#define CMD_STATS "$STATUS"

typedef enum {
    CMD_INVALID,
    CMD_MANUAL,
    CMD_AUTO,
    CMD_STATUS,
    CMD_SET_TEMP

}CMD_t;


uint8_t CLI_BUFFER[CLI_BUFFER_SIZE];
uint8_t CLI_PROCESS_BUFFER[CLI_PROCESS_BUFFER_SIZE];
uint8_t f_ready_to_process = 0 ;
uint8_t process_len = 0;
uint8_t available_len= 0;
int16_t temporary_target = 0;
char * tmp = "";


DMA_Buffer_t CLI_DMA_Buffer = {
    .huart = &huart1,
    .size = CLI_BUFFER_SIZE,
    .buffer = CLI_BUFFER,
    .read_index = 0,
    .f_ready_to_transfer = 0
};
void cli_process ();
void cli_set_mode (CMD_t cmd) ;
void cli_set_temp (uint8_t temp) ;
void cli_get_status () ;
void cli_invalid () ;
CMD_t cli_get_cmd (char* cmd) ;


void cli_init() {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1 , CLI_BUFFER , CLI_BUFFER_SIZE);
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : BOOTING \r\n" , sizeof("Log : BOOTING \r\n") , 20);
}

void cli_loop () {
    if (CLI_DMA_Buffer.f_ready_to_transfer == 1) {

        available_len = available(&CLI_DMA_Buffer);
        CLI_DMA_Buffer.f_ready_to_transfer = 0;

        if (available_len > 0) {
            process_len = copy_to_process_buffer(&CLI_DMA_Buffer , CLI_PROCESS_BUFFER , CLI_PROCESS_BUFFER_SIZE - 1);
            //HAL_GPIO_TogglePin(LED_GPIO_Port , LED_Pin);
            f_ready_to_process = 1 ;

        }else{}
        cli_process () ;

    }else{}

}

void cli_process () {
    if (f_ready_to_process == 1) {
       char * cmd = strstr((char *)(CLI_PROCESS_BUFFER) , "$");
        CMD_t command = cli_get_cmd(cmd);
        switch (command) {
            case CMD_INVALID:
                cli_invalid();
                break;
            case CMD_STATUS:
                cli_get_status();
                break;
            case CMD_SET_TEMP:
                cli_set_temp(temporary_target);
                break;
            case CMD_MANUAL:
                cli_set_mode(CMD_MANUAL);
                break;
            case CMD_AUTO:
                cli_set_mode(CMD_AUTO);
                break;
            default:
                break;
        }

    }
}

CMD_t cli_get_cmd (char* cmd) {
    tmp = strupr(cmd);

    if (strncmp(tmp , CMD_MODE , sizeof(CMD_MODE)-1) == 0) {
        tmp += sizeof(CMD_MODE)-1;
        if (strncmp(tmp , CMD_MODE_MANUAL , sizeof(CMD_MODE_MANUAL)) == 0) {
            return CMD_MANUAL;
        }
        if (strncmp(tmp , CMD_MODE_AUTO , sizeof(CMD_MODE_AUTO)) == 0) {
            return CMD_AUTO;
        }
    }else if (strncmp(tmp , CMD_TEMP , sizeof(CMD_TEMP)-1) == 0) {
        tmp += sizeof(CMD_TEMP) - 1;
        temporary_target = atoi(tmp);
        return CMD_SET_TEMP;
    }else if (strncmp(tmp , CMD_STATS , sizeof(CMD_STATS)) == 0) {
        return CMD_STATUS;
    }else{}

    return CMD_INVALID;
}

//to do must be revamped
void cli_set_mode (CMD_t cmd) {
    switch (cmd) {
        case CMD_MANUAL:
            //set manual
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : MANUAL\r\n" , sizeof("Log : MANUAL\r\n") , 20);
            change_to_manual_mode();
            break;
        case CMD_AUTO:
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : AUTO\r\n" , sizeof("Log : AUTO\r\n") , 20);
            change_to_auto_mode();
            break;
        default:
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : DIDNT SET\r\n" , sizeof("Log : DIDNT SET\r\n") , 20);
            break;
    }
}
void cli_set_temp (uint8_t temp) {
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : SET TEMP\r\n" , sizeof("Log : SET TEMP\r\n") , 20);
    change_target_temp(temporary_target);
}
void cli_get_status () {
    const Status_t st = get_system_status();
    char target_temp_str[24] = "";
    char current_temp_str[24] = "";
    char fan_str[24] = "";
    char* state_str = "\0";

    snprintf((char *)target_temp_str , sizeof(target_temp_str) , "Target temp: %d \r\n", st.target_temp);

    snprintf((char *)current_temp_str , sizeof(current_temp_str) , "current temp: %d \r\n", st.current_temp);

    snprintf((char *)fan_str , sizeof(fan_str) , "fan speed : %d \r\n", st.fan_speed);




    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : STATUS: \r\n" , sizeof("Log : STATUS: \r\n") , 20);
    if (st.mode == MANUAL) {
        HAL_UART_Transmit(&huart1 , (uint8_t *)"MODE = MANUAL \r\n" , sizeof("MODE = MANUAL \r\n") , 20);
    }else{
        HAL_UART_Transmit(&huart1 , (uint8_t *)"MODE = AUTO \r\n" , sizeof("MODE = AUTO \r\n") , 20);}

    HAL_UART_Transmit(&huart1 , (uint8_t *)target_temp_str , sizeof(target_temp_str) , 20);

    HAL_UART_Transmit(&huart1 , (uint8_t *)current_temp_str , sizeof(current_temp_str) , 20);

    HAL_UART_Transmit(&huart1 , (uint8_t *)fan_str , sizeof(fan_str) , 20);

    switch (st.state) {
        case Init:
            state_str = "STATE : Init\r\n\0";
            break;
        case Normal :
            state_str = "STATE : Normal\r\n\0";
            break;
        case Warning:
            state_str = "STATE : Warning\r\n\0";
            break;
        case Error:
            state_str = "STATE : Error\r\n\0";
            break;
    }
    HAL_UART_Transmit(&huart1 , (uint8_t *)state_str , strlen(state_str) , 20);

    if (st.heater_state == heater_off) {
        HAL_UART_Transmit(&huart1 , (uint8_t *)"HEATER OFF \r\n" , sizeof("HEATER OFF \r\n") , 20);
    }else {
        HAL_UART_Transmit(&huart1 , (uint8_t *)"HEATER ON \r\n" , sizeof("HEATER ON \r\n") , 20);
    }




}
void cli_invalid () {
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : INVALID COMMAND \r\n" , sizeof("Log : INVALID COMMAND \r\n") , 20);

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                                uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        CLI_DMA_Buffer.f_ready_to_transfer = 1;
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_GPIO_TogglePin(RGB_GREEN_GPIO_Port, RGB_GREEN_Pin);


    }
}


void StartCliTask(void *argument)
{

    /* Infinite loop */
    for(;;)
    {
        cli_loop();
        osDelay(1);
    }

}


