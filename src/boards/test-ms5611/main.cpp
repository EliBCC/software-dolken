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

#include <common/board.hpp>

#include "ms5611_example.hpp"

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int main(void) {
	initCommon();

	modm::platform::GpioA0::setOutput(modm::Gpio::High);
	modm::platform::SpiMaster1::connect<modm::platform::GpioC1::Mosi, modm::platform::GpioC2::Miso, modm::platform::GpioB10::Sck>();
	modm::platform::SpiMaster1::initialize<ClockConfiguration, 5000000>();
	modm::platform::GpioA6::configure(modm::platform::Gpio::InputType::PullDown);

	MODM_LOG_INFO << "======MS5611 test======" << modm::endl;
	MODM_LOG_DEBUG << "main: debug logging here" << modm::endl;
	MODM_LOG_INFO << "main: info logging here" << modm::endl;
	MODM_LOG_WARNING << "main: warning logging here" << modm::endl;
	MODM_LOG_ERROR << "main: error logging here" << modm::endl;

	Ms5611ExampleThread ms5611ExampleThread;
	while (1) {
		ms5611ExampleThread.run();
	}
	return 0;
}
