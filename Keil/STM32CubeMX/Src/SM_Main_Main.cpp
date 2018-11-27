#include "SM_Main_Main.h"
#include "JAE_include.h"   

void Init(void){
	//test function by jae
	
	JAE::max(2,3);
	JAE::Vector<float> v = JAE::Vector<float>();
	
	
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	JAE::Callback_Delay(1000, ToggleBlink);
}

void Update(void){
	//test function by jae
	
	if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	
}

void ToggleBlink(void){
	
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	
	JAE::Callback_Delay(1000, ToggleBlink);
}
