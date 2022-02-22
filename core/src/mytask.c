#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks

unsigned long int* pt_counter = 0;
unsigned long int counter = 0 ;

void print (void *uart){
    char msg[36];
    
    vTaskDelay(period); // wait for pt_counter to be allocated

    while (1){
        // timing code
        // for some obscure reason the delay until causes issues
        // vTaskDelayUntil(&lastWakeTime, period);
        vTaskDelay(period);

        if (pt_counter) {
            sprintf(msg, "Counter in HEAP  Iterations: %ld \n\r", *pt_counter);
            *pt_counter = 0;
        } else {
            sprintf(msg, "Counter in STACK Iterations: %ld \n\r", counter);
            counter = 0;
        }
        
        // print message to serial
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        // toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    }
}

/* increment counter stored in HEAP with malloc */
void IncrementCounterPointer(){

    pt_counter = (unsigned long int *) pvPortMalloc(sizeof(int));

    while (1) {
        (*pt_counter)++;
    }
}

/* increment counter stored in stack */
void IncrementCounter(){

    while (1) {
        counter++;
    }
}