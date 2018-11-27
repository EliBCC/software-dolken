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


#include <common/board.hpp>
#include <common/config.hpp>
#include <modm/debug/logger.hpp>

using namespace modm::platform;

modm::IODeviceWrapper<Usart2, modm::IOBuffer::BlockIfFull> loggerDevice;
modm::log::Logger modm::log::info(loggerDevice);

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int main(void) {
	Board::initialize();


	while (1)
	{
	}

	while (true) {}
	return 0;
}
