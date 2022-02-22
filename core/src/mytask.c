#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks

unsigned long int* pt_counter = NULL;
unsigned long int counter = 0 ;
int flag = 0;

void print (void *uart){
    char msg[36];
    
    vTaskDelay(period); // wait for pt_counter to be allocated

    while (1){
        // timing code
        // for some obscure reason the delay until causes issues
        // vTaskDelayUntil(&lastWakeTime, period);
        vTaskDelay(period);

        if (flag) {
            sprintf(msg, "Counter in HEAP  Iterations: %ld \n\r", *pt_counter);
        } else {
            sprintf(msg, "Counter in STACK Iterations: %ld \n\r", counter);
        }
        // print message to serial
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        // toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

        //reset counters (done here for more "fair" comparison)
        pt_counter = (unsigned long int *) pvPortMalloc(sizeof(int));
        // find it really weird that this casting is needed
        //but otherwise the counter is not reset
        counter = (unsigned long int) 0; 
    }
}

/* increment counter stored in HEAP with malloc */
void IncrementCounterPointer(){

    flag = 1;
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