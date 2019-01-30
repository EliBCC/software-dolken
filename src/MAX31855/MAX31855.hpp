/*
* Written by Jakob Furbo Enevoldsen
*/

#include <common/board.hpp>
#include <modm/platform.hpp>

template<typename SPI, typename CS> 
class MAX31855
{

private:

	void SPIHardwareRead();
	void SPISoftwareRead();
	int SPIread() {


		CS::reset();


	}

public:
	MAX31855() {

		CS::set();
	}

	int readCelsius(){
		
		return 0;
	}


};
