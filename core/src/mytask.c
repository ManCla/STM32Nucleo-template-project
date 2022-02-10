#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks

void hello (void *uart){
    
    TickType_t lastWakeTime = xTaskGetTickCount();
        
    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);
        
        // count periods
        char msg[36];
        sprintf(msg, "-%ld-\n\r", lastWakeTime/period);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}

void compute (){
    TickType_t lastWakeTime = xTaskGetTickCount();
        
    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);

        int busy = 500 + xTaskGetTickCount();
        int i=0;
        while (xTaskGetTickCount()<busy){
            i++;
        }
        factorial(10);
    }
}

int factorial(int i ){
    if (i<=1){
        return i;
    } else {
        return i*factorial(i-1);
    }

}