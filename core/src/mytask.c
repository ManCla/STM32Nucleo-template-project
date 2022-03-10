#include <string.h>
#include <stdio.h>

const int period = 20; // thread period in ticks
TIM_HandleTypeDef htim14;

void hello (void *uart){

    int timer_val;

    /* start timer */
    HAL_TIM_Base_Start(&htim14);
    
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);
        timer_val = __HAL_TIM_GET_COUNTER(&htim14);
        htim14.Instance->CNT =0; //brutally reset the counter

        // count periods
        char msg[36];
        sprintf(msg, "Period in microsec -%d-\n\r", timer_val);
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        
        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}
