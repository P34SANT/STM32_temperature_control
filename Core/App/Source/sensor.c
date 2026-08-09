    //
    // Created by Peasant on 8/4/2026.
    //

    #include "../Include/sensor.h"
    #include "adc.h"
    #include "FreeRTOS.h"
    #include "task.h"
    #include "cmsis_os.h"


    #define VREF              33      //3.3V
    #define SENSITIVITY       1       //1 has no effect
    #define MAF_SAMPLE_RATE   5
    #define SENSOR_MAX_VALUE (4096-1) //2^12 adc resolution
    #define TEMP_UPDATE_INTERVAL_MS (1000)
    #define ADC_DMA_LENGTH (1)
    volatile uint32_t raw_sensor_value = 0;
    volatile uint32_t filtered_sensor_value = 0;
    volatile double temp_sensor_value = 0;
    //moving avg filter vars
    uint32_t  MAF_Counter = 0;
    volatile uint32_t MAF_Sum     = 0;




    double calculate_temp_value(uint32_t raw_value) ;



    void Sensor_Init() {
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(&raw_sensor_value), ADC_DMA_LENGTH);

    }

    void Sensor_update() {



    }

    double calculate_temp_value(uint32_t raw_value) {
        return (raw_value * VREF) / (SENSITIVITY * SENSOR_MAX_VALUE);

    }

    void StartSensorTask(void *argument)
    {

        /* Infinite loop */
        for(;;)
        {
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(&raw_sensor_value), ADC_DMA_LENGTH);
            MAF_Sum += raw_sensor_value;
            MAF_Counter++;
            if (MAF_Counter == MAF_SAMPLE_RATE) {
                filtered_sensor_value = MAF_Sum / MAF_SAMPLE_RATE;
                MAF_Sum = 0;
                MAF_Counter = 0;
                temp_sensor_value = calculate_temp_value(filtered_sensor_value);


            }
            //Sensor_update();
            osDelay(pdMS_TO_TICKS(TEMP_UPDATE_INTERVAL_MS/MAF_SAMPLE_RATE));
        }

    }
