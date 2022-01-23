#include <string.h>

int flag = 0;

void hello (void *uart){
    while (1){
        vTaskDelay(2000);
        char msg[] = "bellazz\n\r";
        HAL_UART_Transmit(uart, (uint8_t *)msg, strlen(msg), 0xFFFF);
        if (flag) {
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET); 
            flag = 0;
        } else {
            HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); 
            flag = 1;
        }
        
    }
}