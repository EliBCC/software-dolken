/*
 * Copyright (c) 2016-2018, Niklas Hauser
 * Copyright (c) 2017, Sascha Schade
 * Copyright (c) 2018, Antal Szabó
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#pragma once

#include <modm/platform.hpp>
#include <modm/architecture/interface/clock.hpp>
#include <modm/debug/logger.hpp>
#include <modm/processing/timer.hpp>

using namespace modm::platform;

#ifndef MODM_BOARD_HAS_LOGGER
// Create an IODeviceWrapper around the Uart Peripheral we want to use
modm::IODeviceWrapper< Usart1, modm::IOBuffer::BlockIfFull > loggerDevice;
// Set all four logger streams to use the UART
modm::log::Logger modm::log::debug(loggerDevice);
modm::log::Logger modm::log::info(loggerDevice);
modm::log::Logger modm::log::warning(loggerDevice);
modm::log::Logger modm::log::error(loggerDevice);
#endif

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

namespace Board {

	/// STM32F446 running at 168MHz from the external 26MHz crystal
	struct systemClock {
		static constexpr uint32_t Frequency = MHz168;
		static constexpr uint32_t Apb1 = Frequency / 4;
		static constexpr uint32_t Apb2 = Frequency / 2;

		static constexpr uint32_t Adc = Apb2;

		static constexpr uint32_t Spi1 = Apb2;
		static constexpr uint32_t Spi2 = Apb1;
		static constexpr uint32_t Spi3 = Apb1;
		static constexpr uint32_t Spi4 = Apb2;
		static constexpr uint32_t Spi5 = Apb2;
		static constexpr uint32_t Spi6 = Apb2;

		static constexpr uint32_t Usart1 = Apb2;
		static constexpr uint32_t Usart2 = Apb1;
		static constexpr uint32_t Usart3 = Apb1;
		static constexpr uint32_t Uart4  = Apb1;
		static constexpr uint32_t Uart5  = Apb1;
		static constexpr uint32_t Usart6 = Apb2;
		static constexpr uint32_t Uart7  = Apb1;
		static constexpr uint32_t Uart8  = Apb1;

		static constexpr uint32_t Can1 = Apb1;
		static constexpr uint32_t Can2 = Apb1;

		static constexpr uint32_t I2c1 = Apb1;
		static constexpr uint32_t I2c2 = Apb1;
		static constexpr uint32_t I2c3 = Apb1;
		static constexpr uint32_t I2c4 = Apb1;

		static constexpr uint32_t Apb1Timer = Apb1 * 2;
		static constexpr uint32_t Apb2Timer = Apb2 * 2;
		static constexpr uint32_t Timer1  = Apb2Timer;
		static constexpr uint32_t Timer2  = Apb1Timer;
		static constexpr uint32_t Timer3  = Apb1Timer;
		static constexpr uint32_t Timer4  = Apb1Timer;
		static constexpr uint32_t Timer5  = Apb1Timer;
		static constexpr uint32_t Timer6  = Apb1Timer;
		static constexpr uint32_t Timer7  = Apb1Timer;
		static constexpr uint32_t Timer8  = Apb2Timer;
		static constexpr uint32_t Timer10 = Apb2Timer;
		static constexpr uint32_t Timer11 = Apb2Timer;
		static constexpr uint32_t Timer12 = Apb1Timer;
		static constexpr uint32_t Timer13 = Apb1Timer;
		static constexpr uint32_t Timer14 = Apb1Timer;

		static bool inline enable() {
			ClockControl::enableExternalCrystal(); // 26 MHz
			// ClockControl::enableInternalClock(); //16 MHz
			ClockControl::enablePll(
				ClockControl::PllSource::ExternalCrystal,
				26,      // 26MHz / M=26 -> 1MHz   !!! Must be 1 MHz for PLLSAI !!!
				336,    // 1MHz * N=336 -> 336MHz
				2,      // 168MHz / P=2 -> 168MHz = F_cpu
				7       // 48MHz / Q=7 -> 48MHz (value ignored! PLLSAI generates 48MHz for F_usb)
			);

			// set flash latency for 168MHz
			ClockControl::setFlashLatency(Frequency);

			// switch system clock to PLL output
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
	};

	using LedOrangeD2 = GpioOutputC8;
	using LedOrangeD3 = GpioOutputC6;

	inline void initialize() {
		systemClock::enable();
		modm::cortex::SysTickTimer::initialize<systemClock>();

		LedOrangeD2::setOutput(modm::Gpio::Low);
		LedOrangeD3::setOutput(modm::Gpio::Low);

		Usart1::connect<GpioA10::Rx, GpioA9::Tx>();
		Usart1::initialize<Board::systemClock, Usart1::B115200>();
	}

}