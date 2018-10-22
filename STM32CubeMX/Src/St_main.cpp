#ifdef __cplusplus
 extern "C" { 
		#include "stm32f4xx_hal.h"
 }
#endif 

#include "St_main.h"
#include "Config.h"
#include "JAE_include.h" 
#include "BoardInfo.h"
#include "main.h"

#if BOARD_COMPILE_STATE == BOARD_MAIN
		#include "SM_Main_Main.h"
#elif BOARD_COMPILE_STATE == BOARD_ACTUATOR
		#include "SM_Actuator_Main.h"
#elif BOARD_COMPILE_STATE == BOARD_MEASUREMENT
		#include "SM_Measurement_Main.h"
#elif BOARD_COMPILE_STATE == BOARD_PSU
		#include "SM_PSU_Main.h"
#elif BOARD_COMPILE_STATE == BOARD_TELEMETRY
		#include "SM_Telemetry_Main.h"
#else
		#include "SM_Test.h"
#endif

#ifndef BOARD_DEF

Board& board = Board::getBoard();

#define BOARD_DEF
#endif

void checkErrorType(int e){
	
	if(e == ERROR_UNEXPECTED){
		board.printDebug("UNEXPECTED ERROR\n");
	} else if(e == ERROR_INVALID_POINTER){
		board.printDebug("ERROR INVALID POINTER\n");
	}else if(e == ERROR_OUT_OF_BOUNDS){
		board.printDebug("ERROR OUT OF BOUNDS\n");
	}
	
}

void InitStMain(UART_HandleTypeDef* uartDebugLine){
	//test function by jae
	
		try{
			board.boardInit();
			board.setDebugLine(uartDebugLine);
		}catch(int e){
			
			board.printDebug("failed to init board\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to init board\n");
			return;
		}
		
		try{
			JAE::init();
		}catch(int e){
			
			board.printDebug("failed to init JAE lib\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to init JAE lib\n");
			return;
		}
		
		board.printDebug("board waking\n");
		
		try{
			Init();
		}catch(int e){
			
			board.printDebug("failed to init controller\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to init controller\n");
			return;
		}
		
		

}

void UpdateStMain(void){
	//test function by jae
		
		try{
			board.boardUpdate();
		}catch(int e){
			
			board.printDebug("failed to update board\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to update board\n");
			return;
		}
		
		try{
			JAE::update();
		}catch(int e){
			
			board.printDebug("failed to update JAE lib\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to update JAE lib\n");
			return;
		}
		
		try{
			Update();
		}catch(int e){
			
			board.printDebug("failed to update\n");
			
			checkErrorType(e);
			
			return;
		}catch(...){
			board.printDebug("failed to update\n");
			return;
		}
		
}
