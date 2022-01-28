#include <string.h>
#include <stdio.h>

const int period_boss = 1000; // thread period in ticks

SemaphoreHandle_t smph;

void boss (void *uart){
    
    TickType_t lastWakeTime = xTaskGetTickCount();
    smph = xSemaphoreCreateBinary();

    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period_boss);
        
        // count periods
        char msg[36];
        sprintf(msg, "BOSS:     -%ld-: toggle! \n\r", lastWakeTime/period_boss);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        xSemaphoreGive(smph);
    }
}

void employee (void *uart) {

    int smph_timeout = 500; // control how many times else branch is taken

    while (1){
        if (xSemaphoreTake(smph, smph_timeout)) {
            //toggle led
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            char msg[36];
            sprintf(msg, "EMPLOYEE: toggled LED \n\r");
            HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        } else {
            char msg[36];
            sprintf(msg, "EMPLOYEE: no semaphore for me \n\r");
            HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        }
    }
}
