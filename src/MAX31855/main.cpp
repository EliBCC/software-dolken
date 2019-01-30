/*
* Written by Jakob Furbo Enevoldsen
*/


#include <modm/architecture/interface/delay.hpp>
#include <modm/debug/logger.hpp>

#include <common/board.hpp>
#include <common/blink_thread.hpp>

#include "MAX31855.hpp"
#include <modm/platform/spi/spi_master_1.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int main(void) {
	initCommon();

	MODM_LOG_INFO << "======ADS7828 test======" << modm::endl;
	MODM_LOG_DEBUG << "main: debug logging here" << modm::endl;
	MODM_LOG_INFO << "main: info logging here" << modm::endl;
	MODM_LOG_WARNING << "main: warning logging here" << modm::endl;
	MODM_LOG_ERROR << "main: error logging here" << modm::endl;

	for (int i = 0; i < 10; i++) {
		LedD2::set();
		LedD3::reset();
		modm::delayMilliseconds(50);

		LedD2::reset();
		LedD3::set();
		modm::delayMilliseconds(50);
	}
	LedD2::set();
	LedD3::set();

	BlinkThread blinkThread;
	while (1) {
		blinkThread.run();
	}
	return 0;
}
