/*
 * DMAbuffer.h
 *
 *  Created on: Aug 6, 2026
 *      Author: Peasant
 */

#ifndef INC_DMABUFFER_H_
#define INC_DMABUFFER_H_
#include "main.h"


//create the struct and buffers
//then simply use availble and copy to process buffer

typedef struct
{
    UART_HandleTypeDef *huart;

    uint8_t *buffer;

    uint16_t size;

    uint16_t read_index ;

    uint16_t f_ready_to_transfer ;

} DMA_Buffer_t;


uint16_t available(DMA_Buffer_t *buffer_handle);

uint16_t copy_to_process_buffer(DMA_Buffer_t* buffer_handle,uint8_t * process_buffer , uint16_t process_size);


#endif /* INC_DMABUFFER_H_ */
