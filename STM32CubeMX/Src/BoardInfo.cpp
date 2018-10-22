#include "BoardInfo.h"
#include "JAE_include.h" 
#include <string.h>

static Board* board;

void Board::boardInit(void){
	
	cycleTime = getMillis();
} 

void Board::boardUpdate(void){
	
	cycleTime = getMillis();
	
}

void Board::setDebugLine(UART_HandleTypeDef* uartDebugLine){
	
	if(uartDebugLine == NULL)
		throw ERROR_INVALID_POINTER;
	
	this->uartDebugLine = uartDebugLine;
}

void Board::printDebug(char message[]){
	
	HAL_UART_Transmit(uartDebugLine, (uint8_t *) message, strlen(message), 10); 
}

long int Board::getCycleMillis(void){
	
	return cycleTime;
}

long int Board::getMillis(void){
	
	return HAL_GetTick();
}


Board& Board::getBoard(void){
	
	if(board == NULL){
		board = new Board();
	}
	
	return *board;
}

Board::Board(){
	
	cycleTime = 0;
}
