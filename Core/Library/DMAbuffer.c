/*
 * RingBuffer.c
 *
 *  Created on: Aug 6, 2026
 *      Author: Peasant
 */


#include "DMAbuffer.h"




uint16_t get_write_index(DMA_Buffer_t *buffer_handle) ;


uint16_t available(DMA_Buffer_t *buffer_handle)
{
	uint16_t write = get_write_index(buffer_handle);

	if(write >= buffer_handle->read_index)
		return write - buffer_handle->read_index;

	return buffer_handle->size - buffer_handle->read_index + write;
}

uint16_t copy_to_process_buffer(DMA_Buffer_t* buffer_handle,uint8_t * process_buffer , uint16_t process_size) {

	uint16_t write = get_write_index(buffer_handle);
	uint16_t counter = 0 ;

	while(buffer_handle->read_index != write && counter < process_size) {

		process_buffer[counter] = buffer_handle->buffer[buffer_handle->read_index];

		buffer_handle->read_index = (buffer_handle->read_index + 1) % buffer_handle->size; //incrementing read_index

		counter++;
	}

	process_buffer[counter] = '\0';
	return counter;//returns length of the string
}


uint16_t get_write_index(DMA_Buffer_t *buffer_handle)
{
	return (buffer_handle->size -
			__HAL_DMA_GET_COUNTER(buffer_handle->huart->hdmarx))
			% buffer_handle->size;
}
