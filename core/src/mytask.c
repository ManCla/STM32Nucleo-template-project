#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks
int adc_value;

void hello (void *uart){
    
    TickType_t lastWakeTime = xTaskGetTickCount();
        
    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);
        
        // count periods
        char msg[36];
        sprintf(msg, "-%d-\n\r", adc_value);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}


void adc_read (void *adc){
    
    TickType_t lastWakeTime = xTaskGetTickCount();
        
    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);
        
        HAL_ADC_Start(adc); 
        HAL_ADC_PollForConversion(adc, 1);
        adc_value = HAL_ADC_GetValue(adc);
        HAL_ADC_Stop(adc);


        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}
