#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks
int adc_value;
int counter=0;

void hello (void *uart){
    
    while (1){
        // timing code
        vTaskDelay(period);
        
        // count periods
        char msg[36];
        sprintf(msg, "Value: %d -- Number of samples: %d \n\r", adc_value, counter);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        counter=0;

        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}


void adc_read (void *adc){
    
    while (1){
    
        HAL_ADC_Start(adc); 
        HAL_ADC_PollForConversion(adc, 1);
        adc_value = HAL_ADC_GetValue(adc);
        HAL_ADC_Stop(adc);
        counter++;

    }
}
