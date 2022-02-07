#include <string.h>
#include <stdio.h>
#include "main.h"

const int period_p1 = 1000; // thread period in ticks
const int period_p2 = 5000; // thread period in ticks
UART_HandleTypeDef huart2;

void producer1 (void * consumer){    
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period_p1);
        
        // count periods
        char msg[36];
        sprintf(msg, "Producer 1 awake \n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 0xFFFF);
        xTaskNotify((TaskHandle_t)consumer, 1, eSetBits);
    }
}

void producer2 (void * consumer){    
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period_p2);
        
        // count periods
        char msg[36];
        sprintf(msg, "Producer 2 awake \n\r");
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 0xFFFF);
        xTaskNotify((TaskHandle_t)consumer, 2, eSetBits);
    }
}

void consumer (void *uart) {
    char msg[36] = "";
    uint32_t read = 0;

    while (1){
        xTaskNotifyWait( 0x00, 0xFF, &read, 50 );
        if (read==1) {
            sprintf(msg, "Notified by 1 \n\r");
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 0xFFFF);
        }
        if (read==2) {
            sprintf(msg, "Notified by 2 \n\r");
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 0xFFFF);
        }
        read = 0;
    }
}
