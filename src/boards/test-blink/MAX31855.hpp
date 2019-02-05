/*
* Written by Jakob Furbo Enevoldsen
*/

#include <common/board.hpp>
#include <modm/platform.hpp>

#include <modm/processing/timer/timeout.hpp>
#include <modm/platform/clock/clock.hpp>
#include <modm/architecture/interface/delay.hpp>

template<typename SPI, typename CS> 
class MAX31855
{

private:

	uint8_t DataIn[4];
	int32_t Datainteger;
	double Celsius; 

	modm::ShortTimeout timeout;

	void SPIread() {

		//set low and wait 1 ms
		CS::reset();
		//timeout.restart(1);
		//PT_WAIT_UNTIL(timeout.isExpired());
		modm::delayNanoseconds(50);

		//read Data
		SPI::transferBlocking(nullptr,&DataIn[0],4);

		//set CS high
		CS::set();
	}

public:
	MAX31855() {

		CS::setOutput();
		CS::set();
	}

	double readCelsius(){

		SPIread();
		
		Datainteger = 0;
		Datainteger += DataIn[0];
		Datainteger = Datainteger << 8;
		Datainteger += DataIn[1];
		Datainteger = Datainteger << 8;
		Datainteger += DataIn[2];
		Datainteger = Datainteger << 8;
		Datainteger += DataIn[3];

		if (Datainteger & 0x7) {
			// uh oh, a serious problem!
			return NAN; 
		}

		if (Datainteger & 0x80000000) {
			// Negative value, drop the lower 18 bits and explicitly extend sign bits.
			Datainteger = 0xFFFFC000 | ((Datainteger >> 18) & 0x00003FFFF);
		}
		else {
			// Positive value, just drop the lower 18 bits.
			Datainteger >>= 18;
		}

		Celsius = ((double)Datainteger) / 4;
		//Celsius = Celsius * 0.25d;

		return Celsius;
	}


};
