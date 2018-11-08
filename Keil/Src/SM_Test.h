#ifdef __cplusplus
 extern "C" { 
		#include "stm32f4xx_hal.h"
 }
#endif 

#ifndef BOARDS_INCLUEDED

void Init(void){
	//test function by jae
		
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}

void Update(void){
	//test function by jae
	
		
		if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		}
		
		HAL_Delay(100);

}


#define BOARDS_INCLUEDED
#endif
