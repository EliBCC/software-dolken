
#ifndef JAE_INCLUDED
#include "Config.h"
#include <string.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>


namespace JAE {
	
//----PIN_DEFINITIONS----//

const uint8_t PIN_PA_0 = 0;
const uint8_t PIN_PA_1 = 1;
const uint8_t PIN_PA_2 = 2;
const uint8_t PIN_PA_3 = 3;
const uint8_t PIN_PA_4 = 4;
const uint8_t PIN_PA_5 = 5;
const uint8_t PIN_PA_6 = 6;
const uint8_t PIN_PA_7 = 7;
const uint8_t PIN_PA_8 = 8;
const uint8_t PIN_PA_9 = 9;
const uint8_t PIN_PA_10 = 10;
const uint8_t PIN_PA_11 = 11;
const uint8_t PIN_PA_12 = 12;
const uint8_t PIN_PA_13 = 13;
const uint8_t PIN_PA_14 = 14;
const uint8_t PIN_PA_15 = 15;


const uint8_t PIN_PB_0 = 16;
const uint8_t PIN_PB_1 = 17;
const uint8_t PIN_PB_2 = 18;
const uint8_t PIN_PB_3 = 19;
const uint8_t PIN_PB_4 = 20;
const uint8_t PIN_PB_5 = 21;
const uint8_t PIN_PB_7 = 22;
const uint8_t PIN_PB_8 = 23;
const uint8_t PIN_PB_9 = 24;
const uint8_t PIN_PB_10 = 25;
const uint8_t PIN_PB_11 = 26;
const uint8_t PIN_PB_12 = 27;
const uint8_t PIN_PB_13 = 28;
const uint8_t PIN_PB_14 = 29;
const uint8_t PIN_PB_15 = 30;


const uint8_t PIN_PC_0 = 32;
const uint8_t PIN_PC_1 = 33;
const uint8_t PIN_PC_2 = 34;
const uint8_t PIN_PC_3 = 35;
const uint8_t PIN_PC_4 = 36;
const uint8_t PIN_PC_5 = 37;
const uint8_t PIN_PC_7 = 38;
const uint8_t PIN_PC_8 = 39;
const uint8_t PIN_PC_9 = 40;
const uint8_t PIN_PC_10 = 41;
const uint8_t PIN_PC_11 = 42;
const uint8_t PIN_PC_12 = 43;
const uint8_t PIN_PC_13 = 44;
const uint8_t PIN_PC_14 = 45;
const uint8_t PIN_PC_15 = 46;

const uint8_t PIN_PH_0 = 64;
const uint8_t PIN_PH_1 = 65;

//--pin def end--//

	
	void Callback_Delay(long int delayMillis, void (*function)());
	
	void init(void);
	void update(void);
	
	void SET_PIN_INPUT_DIGITAL(const uint8_t& pin);
	void SET_PIN_INPUT_ANALOG(const uint8_t& pin);
	void SET_PIN_INPUT_DIGITAL_PULLUP(const uint8_t& pin);
	void SET_PIN_INPUT_ANALOG_PULLUP(const uint8_t& pin);
	void SET_PIN_INPUT_DIGITAL_PULLDOWN(const uint8_t& pin);
	void SET_PIN_INPUT_ANALOG_PULLDOWN(const uint8_t& pin);
	void SET_PIN_OUTPUT(const uint8_t& pin);
	
	void SET_SPI_HANDEL(SPI_HandleTypeDef* handel);
	
	void DEACTIVATE_MPU();
	void ACTIVATE_MPU();
	uint8_t READ_MPU(uint8_t& address);
	
	void SET_PIN_HIGH(const uint8_t& pin);
	void SET_PIN_LOW(const uint8_t& pin);
	void TOGGLE_PIN(const uint8_t& pin);
	
	bool READ_PIN_DIGITAL(const uint8_t& pin);
	bool READ_PIN_ANALOG(const uint8_t& pin);
	
	template <class T>
	class Vector{
		
		private:
				
			T *innerArray;
			int length;
			int capa;
			
		public: 
			
			Vector ()	{
				
				innerArray = new T[5];
				length = 0;
				capa = 5;
			}
			
			~Vector ()	{
				
				delete[] innerArray;
			}
				
			int size ()	{return length;}
			
			int capacity ()	{return capa;}
			
			void resize(const int &newSize)	{
	
				T* newPointer = new T[newSize];
				memcpy(newPointer, innerArray, length * sizeof(T));
				delete[] innerArray;
				innerArray = newPointer;
				
				capa = newSize;
			}
			
			void push_back (const T &newElement)	{
	
				if(length >= capa){
					//if need resize
					resize(capa * 2);
				}
				
				innerArray[length] = newElement;
				length++;
			}
			
			void erase (const int &index)	{
	
				memcpy(&innerArray[index], &innerArray[index+1], (length-index) * sizeof(T));
				length--;
			}


			
			T at (int index)	{
				
				if(index > length)
					throw ERROR_OUT_OF_BOUNDS;
				
				return innerArray[index];
			}
			
	};

	
	template <class T> const T& max (const T& a, const T& b) {
		return (a<b)?b:a;
	}
	
	template <class T> const T& min (const T& a, const T& b) {
		return (a>b)?b:a;
	}
}

#define JAE_INCLUDED
#endif

