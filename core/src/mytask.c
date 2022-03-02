#define DEBUG_MESSAGE(x) char err_msg[64]; \
    sprintf(err_msg, x);        \
    HAL_UART_Transmit(uart, (void*)err_msg,strlen(err_msg), 0xFFFF);

#include <string.h>
#include <stdio.h>

const int period = 1000;          // thread period in ticks
QueueHandle_t readingQueue;       // queue containing incoming bytes
SemaphoreHandle_t printSemaphore; // semaphore to signal printing

void server (void *uart){
    
    // create input queue and printing semaphore
    readingQueue   = xQueueCreate( 64, sizeof( char ) );
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
            if(xQueueSendToBack(readingQueue, (void *)&read, 0) != pdPASS){
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
    char msg;

    while ( 1 ) {

        if ( xSemaphoreTake(printSemaphore, (TickType_t) 100 ) == pdTRUE ) {
            
            while (xQueueReceive(readingQueue,&msg,0) == pdPASS){
                HAL_UART_Transmit(uart, (void*)&msg, 1, 0xFFFF);
            }
            msg='\n\r';
            HAL_UART_Transmit(uart, (void*)&msg, 1, 0xFFFF);
            xQueueReset(readingQueue);
        }
    }
}
