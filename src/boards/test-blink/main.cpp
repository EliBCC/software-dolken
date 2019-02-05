/*
 * Copyright (c) 2013, Kevin Läufer
 * Copyright (c) 2013-2018, Niklas Hauser
 * Copyright (c) 2014, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------


#include <modm/architecture/interface/delay.hpp>
#include <modm/debug/logger.hpp>

#include <modm/platform/clock/clock.hpp>

#include <common/board.hpp>
#include "modm/platform/spi/spi_master_1.hpp"
#include "modm/platform/spi/spi_master_2.hpp"
#include "modm/platform.hpp"

#include "MAX31855.hpp"

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int float_to_int(float f)
{
    return static_cast<int>(f); // has no side-effects
}

static double temp = 0;

int main(void) {
	initCommon();

	MODM_LOG_DEBUG << "main: debug logging here" << modm::endl;
	MODM_LOG_INFO << "main: info logging here" << modm::endl;
	MODM_LOG_WARNING << "main: warning logging here" << modm::endl;
	MODM_LOG_ERROR << "main: error logging here" << modm::endl;

	for (int i = 0; i < 10; i++) {
		LedD2::set();
		LedD3::reset();
		modm::delayMilliseconds(50 * temp + 50);

		LedD2::reset();
		LedD3::set();
		modm::delayMilliseconds(50);
	}
	LedD2::set();
	LedD3::set();

	modm::platform::SpiMaster2::connect<modm::platform::GpioOutputB13::Sck, modm::platform::GpioOutputB14::Miso, modm::platform::GpioOutputB15::Mosi>();
	modm::platform::SpiMaster2::initialize<ClockConfiguration, 656250, modm::Tolerance::TwentyPercent>();
	
	auto maxInstance = MAX31855<modm::platform::SpiMaster2, modm::platform::GpioOutputB1>();
	
	while (1) {
		temp = maxInstance.readCelsius();
		MODM_LOG_INFO.printf("welcome");
	}
	return 0;
}
