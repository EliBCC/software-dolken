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
#include <modm/processing/protothread.hpp>

#include <common/board.hpp>
#include <common/blink_thread.hpp>
#include <common/can-protocol.hpp>

#include "measurement.hpp"
#include "actuator.hpp"
#include "can-thread.hpp"

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

CanThread canThread;
MeasurementThread measurementThread;
ActuatorThread actuatorThread;
BlinkThread blinkThread;

int main(void) {
	initCommon();

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

	while (1) {
		blinkThread.run();
		canThread.run();
		measurementThread.run();
		actuatorThread.run();
	}
	return 0;
}
