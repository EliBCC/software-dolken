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

#ifndef MODM_STM32_NUCLEO_F446_HPP
#define MODM_STM32_NUCLEO_F446_HPP

#include <modm/platform.hpp>
#include <modm/architecture/interface/clock.hpp>

/// @ingroup modm_board_disco_f469ni
#define MODM_BOARD_HAS_LOGGER

/// @ingroup modm_board_disco_f469ni
namespace Board
{

	/// STM32F469 running at 180MHz from the external 8MHz crystal
	struct systemClock
	{
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

		static bool enable(void);
	};

	void initialize(void);

}

#endif	// MODM_STM32_F469_DISCOVERY_HPP
