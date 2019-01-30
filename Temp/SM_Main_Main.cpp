#include "SM_Main_Main.h"
#include "JAE_include.h"   
#include "BoardInfo.h"

void Init(void){
	//test function by jae
	
	JAE::SET_PIN_INPUT_DIGITAL(JAE::PIN_PC_13);
	
	JAE::TOGGLE_PIN(JAE::PIN_PA_5);
	JAE::Callback_Delay(1000, ToggleBlink);
}

void Update(void){
	//test function by jae
	
	if(!JAE::READ_PIN_DIGITAL(JAE::PIN_PC_13)){
		JAE::SET_PIN_HIGH(JAE::PIN_PA_7);
	}else{
		JAE::SET_PIN_LOW(JAE::PIN_PA_7);
	}
	
}

void ToggleBlink(void){
	
	JAE::TOGGLE_PIN(JAE::PIN_PA_6);
	JAE::TOGGLE_PIN(JAE::PIN_PA_5);
	
	JAE::Callback_Delay(1000, ToggleBlink);
}