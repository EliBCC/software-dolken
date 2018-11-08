
#ifndef JAE_INCLUDED
#include "Config.h"
#include <string.h>

namespace JAE {
	
	void Callback_Delay(long int delayMillis, void (*function)());
	
	void init(void);
	void update(void);
	
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

