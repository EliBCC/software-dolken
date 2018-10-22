#ifdef __cplusplus
 extern "C" { 
		#include "stm32f4xx_hal.h"
 }
#endif 

class Board{
	
	private: 
		Board(const Board&);
		Board();
		
		long int cycleTime;
		UART_HandleTypeDef* uartDebugLine;
	
	public:
		static Board& getBoard(void);
		
		void boardInit(void);
		void boardUpdate(void); 
	
		void setDebugLine(UART_HandleTypeDef* uartDebugLine);
		void printDebug(char message[]);

		long int getCycleMillis(void); //gets the time from start to the start of the current cycle in millis
		long int getMillis(void); //get the tome from start to now

};
