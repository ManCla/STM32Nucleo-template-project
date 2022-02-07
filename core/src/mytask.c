#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks

void hello (void *uart){
    vTaskSetApplicationTaskTag( NULL, ( void * ) 5 );
    TickType_t lastWakeTime = xTaskGetTickCount();
        
    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);

    }
}

void t1 (void *uart){
    vTaskSetApplicationTaskTag( NULL, ( void * ) 1 );
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (1){
        // timing code
        vTaskDelayUntil(&lastWakeTime, period);
        
        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
}
