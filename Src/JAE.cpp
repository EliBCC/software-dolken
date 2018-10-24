#include "JAE_include.h" 
#include <string.h> 
#include "BoardInfo.h"
#include "Config.h"

static JAE::Vector<void (*)()> CallbackFunctionVector;
static JAE::Vector<long int> CallbackTimeVector;

int getPinName(uint8_t pin){
	
	if(pin == 0){
		return GPIO_PIN_0;
	}
	if(pin == 1){
		return GPIO_PIN_1;
	}
	if(pin == 2){
		return GPIO_PIN_2;
	};
	if(pin == 3){
		return GPIO_PIN_3;
	}
	if(pin == 4){
		return GPIO_PIN_4;
	}
	if(pin == 5){
		return GPIO_PIN_5;
	}
	if(pin == 6){
		return GPIO_PIN_6;
	}
	if(pin == 7){
		return GPIO_PIN_7;
	}
	if(pin == 8){
		return GPIO_PIN_8;
	}
	if(pin == 9){
		return GPIO_PIN_9;
	}
	if(pin == 10){
		return GPIO_PIN_10;
	}
	if(pin == 11){
		return GPIO_PIN_11;
	}
	if(pin == 12){
		return GPIO_PIN_12;
	}
	if(pin == 13){
		return GPIO_PIN_13;
	}
	if(pin == 14){
		return GPIO_PIN_14;
	}
	if(pin == 15){
		return GPIO_PIN_15;
	}
	
	throw ERROR_INVALID_PIN;
	
}

GPIO_TypeDef* getPinGroup(uint8_t pin){
	
	uint8_t groupNum = (pin >> 4);

	if(groupNum == 0){
		return GPIOA;
	}
	
	if(groupNum == 1){
		return GPIOB;
	}
	
	if(groupNum == 2){
		return GPIOC;
	}
		
	if(groupNum == 3){
		return GPIOH;
	}
	
	Board::getBoard().printDebug("was not found");
	
	throw ERROR_INVALID_PIN;
	
}

void JAE::Callback_Delay(long int delayMillis, void (*function)()){
	
	Board& board = Board::getBoard();
	
	CallbackFunctionVector.push_back(function);
	CallbackTimeVector.push_back(delayMillis + board.getCycleMillis());
}

void JAE::init(void){
	
	
}

void JAE::update(void)	{
	
	
	//check if should call callbackfunctions, then call it and delete it
	
	for(int i = 0; i < CallbackFunctionVector.size(); i++){
		
		Board& board = Board::getBoard();
		
		if(CallbackTimeVector.at(i) <= board.getCycleMillis()){
			CallbackFunctionVector.at(i)();
			CallbackFunctionVector.erase(i);
			CallbackTimeVector.erase(i);
		}
	}

}

void JAE::SET_PIN_INPUT_DIGITAL(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}

	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_INPUT_ANALOG(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_INPUT_DIGITAL_PULLUP(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_INPUT_ANALOG_PULLUP(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_INPUT_DIGITAL_PULLDOWN(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_INPUT_ANALOG_PULLDOWN(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}


void JAE::SET_PIN_OUTPUT(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	if(g == GPIOA){
		__GPIOA_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in A");
	}
	if(g == GPIOB){
		__GPIOB_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in B");
	}
	if(g == GPIOC){
		__GPIOC_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in C");
	}
	if(g == GPIOH){
		__GPIOH_CLK_ENABLE();
		Board::getBoard().printDebug("Enable a pin in H");
	}
	
	int PIN_NAME = getPinName(number);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = PIN_NAME;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(g, &GPIO_InitStruct);
	
}

void JAE::SET_PIN_HIGH(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	HAL_GPIO_WritePin(g, PIN_NAME, GPIO_PIN_SET);
}

void JAE::SET_PIN_LOW(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	HAL_GPIO_WritePin(g, PIN_NAME, GPIO_PIN_RESET);
}

void JAE::TOGGLE_PIN(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	HAL_GPIO_TogglePin(g, PIN_NAME);
}


bool JAE::READ_PIN_DIGITAL(uint8_t pin){
	
	uint8_t number =  (pin & (uint8_t)15);
	GPIO_TypeDef* g = getPinGroup(pin);
	
	int PIN_NAME = getPinName(number);
	
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){
		return true;
	}
	
	return false;
}





