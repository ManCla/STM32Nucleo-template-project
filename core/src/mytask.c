#define DEBUG_MESSAGE(x) char err_msg[64]; \
    sprintf(err_msg, x);        \
    HAL_UART_Transmit(uart, (void*)err_msg,strlen(err_msg), 0xFFFF);

#define BUFFER_LENGTH 64

#include <string.h>
#include <stdio.h>

const int period = 1000;          // thread period in ticks
QueueHandle_t inQueue;       // queue containing incoming bytes
SemaphoreHandle_t printSemaphore; // semaphore to signal printing

void server (void *uart){
    
    // create input queue and printing semaphore
    inQueue   = xQueueCreate( BUFFER_LENGTH, sizeof( char ) );
    printSemaphore = xSemaphoreCreateBinary();

    // assuming creation of queue and semaphore was successful

    while (1){

        // prompt message
        char msg[64];
        sprintf(msg, "\n\rwrite something:\n\r");
        HAL_UART_Transmit(uart, (void*)msg, strlen(msg), 0xFFFF);

        // read chars one by one
        char read;
        do {
            HAL_UART_Receive(uart, (void*)&read, 1, 0xFFFF);
            if(xQueueSendToBack(inQueue, (void *)&read, 0) != pdPASS){
                DEBUG_MESSAGE("\n\r ### WARNING: input queue is full ### \n\r")
                break;
            }
        } while (read != '\r');

        xSemaphoreGive(printSemaphore);
        vTaskDelay(period);

        //toggle led each time we have finished receiving a string
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    }
}


void printer (void *uart){
    char msg[BUFFER_LENGTH+1]; // make sure that there is space for newline char
    int i=0;
    while ( 1 ) {

        if ( xSemaphoreTake(printSemaphore, (TickType_t) 100 ) == pdTRUE ) {
            i=0;
            while (xQueueReceive(inQueue,msg+i,0)==pdPASS && i<BUFFER_LENGTH) ++i;
            msg[i]='\r';
            HAL_UART_Transmit(uart, (void*)msg, i, 0xFFFF);
            
        }
    }
}
