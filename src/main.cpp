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


#include "Common/Board.hpp"
#include "Common/Config.hpp"
#include <modm/debug/logger.hpp>

using namespace modm::platform;

modm::IODeviceWrapper< Usart2, modm::IOBuffer::BlockIfFull > loggerDevice;
modm::log::Logger modm::log::info(loggerDevice);

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

//checks what board to compile for (and includes that boards init and update func)
#if BOARD_COMPILE_STATE == BOARD_MAIN
	#include "MainBoard/Main.hpp"
#elif BOARD_COMPILE_STATE == BOARD_ACTUATOR
	#include "MainBoard/Main.hpp"
#elif BOARD_COMPILE_STATE == BOARD_MEASUREMENT
	#include "MainBoard/Main.hpp"
#elif BOARD_COMPILE_STATE == BOARD_TELEMETRY
	#include "MainBoard/Main.hpp"
#elif BOARD_COMPILE_STATE == BOARD_TEST
	#include "MainBoard/Main.hpp"
#endif

int main(void) {
	Board::initialize();

	Init();

	while (1)
	{
		Update();
	}

	while (true) {}
	return 0;
}
