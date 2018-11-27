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


#include "board.hpp"
#include <modm/debug/logger.hpp>

#include <common/board.hpp>

bool Board::systemClock::enable(void) 	{
	using namespace modm::platform;
	ClockControl::enableExternalCrystal(); // 8 MHz
	ClockControl::enablePll(
	ClockControl::PllSource::ExternalCrystal,
		8,      // 8MHz / N=8 -> 1MHz   !!! Must be 1 MHz for PLLSAI !!!
		336,    // 1MHz * M=336 -> 336MHz
		2,      // 336MHz / P=2 -> 168MHz = F_cpu
		7       // 336MHz / Q=7 -> 48MHz (value ignored! PLLSAI generates 48MHz for F_usb)
	);

	ClockControl::setFlashLatency(Frequency);
	ClockControl::enableSystemClock(ClockControl::SystemClockSource::Pll);
	ClockControl::setApb1Prescaler(ClockControl::Apb1Prescaler::Div4);
	ClockControl::setApb2Prescaler(ClockControl::Apb2Prescaler::Div2);
	// update clock frequencies
	modm::clock::fcpu     = Frequency;
	modm::clock::fcpu_kHz = Frequency / 1000;
	modm::clock::fcpu_MHz = Frequency / 1000000;
	modm::clock::ns_per_loop = ::round(3000.f / (Frequency / 1000000));
	return true;
}


void Board::initialize(void){
	systemClock::enable();
	modm::cortex::SysTickTimer::initialize<systemClock>();
}