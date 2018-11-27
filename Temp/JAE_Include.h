
#ifndef JAE_INCLUDED
#include "Config.h"
#include <string.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

namespace JAE {
	
//----PIN_DEFINITIONS----//

static uint8_t PIN_PA_0 = 0;
static uint8_t PIN_PA_1 = 1;
static uint8_t PIN_PA_2 = 2;
static uint8_t PIN_PA_3 = 3;
static uint8_t PIN_PA_4 = 4;
static uint8_t PIN_PA_5 = 5;
static uint8_t PIN_PA_6 = 6;
static uint8_t PIN_PA_7 = 7;
static uint8_t PIN_PA_8 = 8;
static uint8_t PIN_PA_9 = 9;
static uint8_t PIN_PA_10 = 10;
static uint8_t PIN_PA_11 = 11;
static uint8_t PIN_PA_12 = 12;
static uint8_t PIN_PA_13 = 13;
static uint8_t PIN_PA_14 = 14;
static uint8_t PIN_PA_15 = 15;


static uint8_t PIN_PB_0 = 16;
static uint8_t PIN_PB_1 = 17;
static uint8_t PIN_PB_2 = 18;
static uint8_t PIN_PB_3 = 19;
static uint8_t PIN_PB_4 = 20;
static uint8_t PIN_PB_5 = 21;
static uint8_t PIN_PB_7 = 22;
static uint8_t PIN_PB_8 = 23;
static uint8_t PIN_PB_9 = 24;
static uint8_t PIN_PB_10 = 25;
static uint8_t PIN_PB_11 = 26;
static uint8_t PIN_PB_12 = 27;
static uint8_t PIN_PB_13 = 28;
static uint8_t PIN_PB_14 = 29;
static uint8_t PIN_PB_15 = 30;


static uint8_t PIN_PC_0 = 32;
static uint8_t PIN_PC_1 = 33;
static uint8_t PIN_PC_2 = 34;
static uint8_t PIN_PC_3 = 35;
static uint8_t PIN_PC_4 = 36;
static uint8_t PIN_PC_5 = 37;
static uint8_t PIN_PC_7 = 38;
static uint8_t PIN_PC_8 = 39;
static uint8_t PIN_PC_9 = 40;
static uint8_t PIN_PC_10 = 41;
static uint8_t PIN_PC_11 = 42;
static uint8_t PIN_PC_12 = 43;
static uint8_t PIN_PC_13 = 44;
static uint8_t PIN_PC_14 = 45;
static uint8_t PIN_PC_15 = 46;

static uint8_t PIN_PH_0 = 64;
static uint8_t PIN_PH_1 = 65;

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

