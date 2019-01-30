/*
* Written by Jakob Furbo Enevoldsen
*/

#include "common/board.hpp"
#include "MAX31855/MAX31855.hpp"
#include <modm/architecture/interface/spi.hpp>

	inline void SPIHardwareRead();
	inline void SPISoftwareRead();
	inline int SPIread() {

		return 0;
	}

template<typename SO, typename CLK, typename CS> 
MAX31855::MAX31855(){

	CS::set();
}