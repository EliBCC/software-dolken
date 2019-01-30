/*
 * Copyright (c) 2016-2018, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_GPIO_BASE_HPP
#define MODM_STM32_GPIO_BASE_HPP

#include "../device.hpp"
#include <modm/architecture/interface/gpio.hpp>
#include <modm/math/utils/bit_operation.hpp>

namespace modm
{

namespace platform
{

/// @cond
enum class
Peripheral
{
	BitBang,
	Adc1,
	Adc2,
	Adc3,
	Can1,
	Can2,
	Crc,
	Dac,
	Dcmi,
	Dma1,
	Dma2,
	Fmpi2c1,
	HdmiCec,
	I2c1,
	I2c2,
	I2c3,
	I2s,
	I2s1,
	I2s2,
	I2s3,
	Iwdg,
	None,
	Quadspi,
	Rcc,
	Rtc,
	Sai1,
	Sdio,
	Spdifrx,
	Spi1,
	Spi2,
	Spi3,
	Sys,
	Tim1,
	Tim10,
	Tim11,
	Tim12,
	Tim13,
	Tim14,
	Tim2,
	Tim3,
	Tim4,
	Tim5,
	Tim6,
	Tim7,
	Tim8,
	Tim9,
	Uart4,
	Uart5,
	Usart1,
	Usart2,
	Usart3,
	Usart6,
	UsbOtgFs,
	UsbOtgHs,
	Wwdg,
};
/// @endcond

/// @ingroup modm_platform_gpio
struct Gpio
{
	enum class
	InputType
	{
		Floating = 0x0,	///< floating on input
		PullUp = 0x1,	///< pull-up on input
		PullDown = 0x2,	///< pull-down on input
	};

	enum class
	OutputType
	{
		PushPull = 0x0,		///< push-pull on output
		OpenDrain = 0x1,	///< open-drain on output
	};

	enum class
	OutputSpeed
	{
		Low      = 0,
		Medium   = 0x1,
		High     = 0x2,
		VeryHigh = 0x3,		///< 30 pF (80 MHz Output max speed on 15 pF)
		MHz2     = Low,
		MHz25    = Medium,
		MHz50    = High,
		MHz100   = VeryHigh,
	};

	enum class
	InputTrigger
	{
		RisingEdge,
		FallingEdge,
		BothEdges,
	};

	/// The Port a Gpio Pin is connected to.
	enum class
	Port
	{
		A = 0,
		B = 1,
		C = 2,
		D = 3,
		H = 7,
	};

	/// @cond
	enum class
	Signal
	{
		BitBang,
		Af1,
		Af2,
		Bk1Io0,
		Bk1Io1,
		Bk1Io3,
		Bk1Ncs,
		Bk2Ncs,
		Bkin,
		Cec,
		Ch1,
		Ch1n,
		Ch2,
		Ch2n,
		Ch3,
		Ch3n,
		Ch4,
		Ck,
		Ckin,
		Clk,
		Cmd,
		Cts,
		D0,
		D1,
		D10,
		D11,
		D2,
		D3,
		D4,
		D5,
		D6,
		D7,
		D8,
		D9,
		Dm,
		Dp,
		Etr,
		FsA,
		FsB,
		Hsync,
		Id,
		In0,
		In1,
		In10,
		In11,
		In12,
		In13,
		In14,
		In15,
		In2,
		In3,
		In4,
		In5,
		In6,
		In7,
		In8,
		In9,
		Jtck,
		Jtdi,
		Jtdo,
		Jtms,
		Jtrst,
		Mck,
		MclkB,
		Mco1,
		Mco2,
		Miso,
		Mosi,
		Nss,
		Osc32In,
		Osc32Out,
		OscIn,
		OscOut,
		Out1,
		Out2,
		Pixclk,
		Refin,
		Rts,
		Rx,
		Sck,
		SckA,
		SckB,
		Scl,
		Sd,
		SdA,
		SdB,
		Sda,
		Smba,
		Sof,
		Swclk,
		Swdio,
		Swo,
		Traced0,
		Tx,
		UlpiCk,
		UlpiD0,
		UlpiD1,
		UlpiD2,
		UlpiD3,
		UlpiD4,
		UlpiD5,
		UlpiD6,
		UlpiD7,
		UlpiDir,
		UlpiNxt,
		UlpiStp,
		Vbus,
		Vsync,
		Wkup0,
		Wkup1,
		Ws,
	};
	/// @endcond

protected:
	/// @cond
	/// I/O Direction Mode values for this specific pin.
	enum class
	Mode
	{
		Input  = 0x0,
		Output = 0x1,
		AlternateFunction = 0x2,
		Analog = 0x3,
		Mask   = 0x3,
	};

	static constexpr uint32_t
	i(Mode mode) { return uint32_t(mode); }
	// Enum Class To Integer helper functions.
	static constexpr uint32_t
	i(InputType pull) { return uint32_t(pull); }
	static constexpr uint32_t
	i(OutputType out) { return uint32_t(out); }
	static constexpr uint32_t
	i(OutputSpeed speed) { return uint32_t(speed); }
	/// @endcond
};

} // namespace platform

} // namespace modm

#endif // MODM_STM32_GPIO_BASE_HPP