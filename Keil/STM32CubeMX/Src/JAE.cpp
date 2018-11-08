#include "JAE_include.h" 
#include <string.h> 
#include "BoardInfo.h"
#include "Config.h"

static JAE::Vector<void (*)()> CallbackFunctionVector;
static JAE::Vector<long int> CallbackTimeVector;

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




