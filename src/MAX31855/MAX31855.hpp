/*
* Written by Jakob Furbo Enevoldsen
*/

#include <common/board.hpp>
#include <modm/platform.hpp>

template<typename SPI, typename CS> 
class MAX31855
{

private:

	void SPIHardwareRead(){

		/*SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

		d = SPI::transfer(0);
		d <<= 8;
		d |= SPI::transfer(0);
		d <<= 8;
		d |= SPI::transfer(0);
		d <<= 8;
		d |= SPI::transfer(0);

		SPI::endTransaction();
		*/

	}

	void SPISoftwareRead();
	
	int SPIread() {

		//reset and wait
		CS::reset();
		//waitms(1)

		SPIHardwareRead();

		return 0;
	}

public:
	MAX31855() {

		CS::set();
	}

	int readCelsius(){
		
		return 0;
	}


};
