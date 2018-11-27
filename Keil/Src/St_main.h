#include "stm32f4xx_hal.h"

#ifndef ST_MAIN_H

void InitStMain(UART_HandleTypeDef* uartDebugLine, SPI_HandleTypeDef* hspi1);
void UpdateStMain(void);

#define ST_MAIN_H
#endif

