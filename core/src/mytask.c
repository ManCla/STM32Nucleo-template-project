#include <string.h>
#include <stdio.h>

const int period_echo  = 10000; // thread period in ticks
const int period_count = 1000; // thread period in ticks

void echo (void *uart){
    while (1){
        // timing code
        TickType_t lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, period_echo);
        
        // receive message
        char in[36];
        HAL_UART_Receive(uart, (uint8_t *)in, strlen(in), 0xFFFF);
        // echo message
        HAL_UART_Transmit(uart, (uint8_t *)in, strlen(in), 0xFFFF);
    }
}

void count (void *uart){
    while (1){
        // timing code
        TickType_t lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, period_count);
        
        //count seconds
        char msg[36];
        sprintf(msg, "-%ld-\n\r", xTaskGetTickCount()/1000);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        //toggle led
        // HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}