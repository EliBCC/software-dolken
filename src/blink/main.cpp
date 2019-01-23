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

#include <common/blink_thread.hpp>
#include <common/board.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int main(void) {
	initCommon();

	MODM_LOG_DEBUG << "debug logging enabled" << modm::endl;
	MODM_LOG_INFO << "info logging enabled" << modm::endl;
	MODM_LOG_WARNING << "warning logging enabled" << modm::endl;
	MODM_LOG_ERROR << "error logging enabled" << modm::endl;

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
