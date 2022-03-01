#include <string.h>
#include <stdio.h>

const int period = 1000; // thread period in ticks

void receive (void *uart){
    
    while (1){
        char received[36];

        // prompt message
        char msg[36];
        sprintf(msg, "\n\rwrite something:\n\r");
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);

        // wait for message
        char read;
        int i=0;
        do {
            HAL_UART_Receive(uart, (uint8_t*) &read, 1, 0xFFFF);
            received[i]=read;
            i++;
        } while (read != '\r');

        // echo
        HAL_UART_Transmit(uart, (uint8_t *)received, strlen(received), 0xFFFF);
        
        vTaskDelay(period);

        //toggle led
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    }
}


void worker (void *uart){

    while (1){
        char msg[36];
        sprintf(msg, "write something\n\r");
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);


    }
}
