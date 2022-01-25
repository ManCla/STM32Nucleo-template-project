#include <string.h>
#include <stdio.h>

const int period_display  = 1000; // thread period in ticks
const int period_up       = 1000; // thread period in ticks
const int period_down     = 1000; // thread period in ticks

char in[8];
int step = 0;

void display (void *uart){
    while (1){
        // timing code
        TickType_t lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, period_display);
        
        char msg[36];
        sprintf(msg, "I am at step: %d \n\r", step);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
    }
}

void climbUP (void *uart){
    while (1){
        // timing code
        TickType_t lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, period_up);

        step++;
        
        // set led
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    }
}

void climbDOWN (void *uart){
    while (1){
        // timing code
        TickType_t lastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil(&lastWakeTime, period_down);

        step--;

        // reset led
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
}