//
// Created by Peasant on 8/4/2026.
//

#include "../Include/cli.h"

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
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : BOOTING " , sizeof("Log : BOOTING ") , 20);
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
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : MANUAL" , sizeof("Log : MANUAL") , 20);
            break;
        case CMD_AUTO:
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : AUTO" , sizeof("Log : AUTO") , 20);
            break;
        default:
            HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : DIDNT SET" , sizeof("Log : DIDNT SET") , 20);
            break;
    }
}
void cli_set_temp (uint8_t temp) {
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : SET TEMP" , sizeof("Log : SET TEMP") , 20);
    change_target_temp(temporary_target);
}
void cli_get_status () {
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : STATUS " , sizeof("Log : STATUS ") , 20);
}
void cli_invalid () {
    HAL_UART_Transmit(&huart1 , (uint8_t *)"Log : INVALID COMMAND " , sizeof("Log : INVALID COMMAND ") , 20);

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,
                                uint16_t Size)
{
    if(huart->Instance == USART1)
    {
        CLI_DMA_Buffer.f_ready_to_transfer = 1;
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);


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


