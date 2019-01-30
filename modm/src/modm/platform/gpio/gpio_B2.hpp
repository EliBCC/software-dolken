/*
 * Copyright (c) 2017-2018, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_GPIO_PIN_B2_HPP
#define MODM_STM32_GPIO_PIN_B2_HPP

#include "../device.hpp"
#include "base.hpp"
#include "set.hpp"

namespace modm
{

namespace platform
{

/// @cond
class GpioB2;
using GpioOutputB2 = GpioB2;
using GpioInputB2  = GpioB2;
/// @endcond

/// IO class for Pin B2
/// @ingroup	modm_platform_gpio
class GpioB2 : public Gpio, public ::modm::GpioIO
{
	template<class... Gpios>
	friend class GpioSet;
	using PinSet = GpioSet<GpioB2>;
	friend class Adc1; friend class Adc2;
	friend class Adc3; friend class Adc4;
public:
	using Output = GpioB2;
	using Input = GpioB2;
	using IO = GpioB2;
	using Type = GpioB2;
	static constexpr bool isInverted = false;
	static constexpr Port port = Port::B; ///< Port name
	static constexpr uint8_t pin = 2; ///< Pin number

protected:
	/// Bitmask for registers that contain a 1bit value for every pin.
	static constexpr uint16_t mask  = 0x1 << pin;
	/// Bitmask for registers that contain a 2bit value for every pin.
	static constexpr uint32_t mask2 = 0x3 << (pin * 2);
	/// Port Number.
	static constexpr uint8_t port_nr = uint8_t(port);
	/// Alternate Function register id. 0 for pin 0-7. 1 for pin 8-15.
	static constexpr uint8_t af_id  = pin / 8;
	/// Alternate Function offset.
	static constexpr uint8_t af_offset = (pin * 4) % 32;
	/// Alternate Function register mask.
	static constexpr uint32_t af_mask  = 0xf << af_offset;
	/// ExternalInterruptIRQ
	static constexpr IRQn_Type ExternalInterruptIRQ = EXTI2_IRQn;

public:
	/// @cond
	inline static void setAlternateFunction(uint8_t af) {
		GPIOB->AFR[af_id] = (GPIOB->AFR[af_id] & ~af_mask) | ((af & 0xf) << af_offset);
		GPIOB->MODER = (GPIOB->MODER & ~mask2) | (i(Mode::AlternateFunction) << (pin * 2));
	}

	/// Enable Analog Mode which is needed to use this pin as an ADC input.
	inline static void setAnalogInput() { PinSet::setAnalogInput(); }
	/// @endcond

public:
	// GpioOutput
	// start documentation inherited
	inline static void setOutput() { PinSet::setOutput(); }
	inline static void setOutput(bool status) { PinSet::setOutput(status); }
	inline static void set() { PinSet::set(); }
	inline static void set(bool status) { PinSet::set(status); }
	inline static void reset() { PinSet::reset(); }
	inline static void toggle() {
		if (isSet()) reset();
		else         set();
	}
	inline static bool isSet() { return (GPIOB->ODR & mask); }
	// stop documentation inherited
	inline static void configure(OutputType type, OutputSpeed speed = OutputSpeed::MHz50) { PinSet::configure(type, speed); }
	inline static void setOutput(OutputType type, OutputSpeed speed = OutputSpeed::MHz50)  { PinSet::setOutput(type, speed); }
	// GpioInput
	// start documentation inherited
	inline static void setInput() { PinSet::setInput(); }
	inline static bool read() { return (GPIOB->IDR & mask); }
	// end documentation inherited
	inline static void configure(InputType type) { PinSet::configure(type); }
	inline static void setInput(InputType type) { PinSet::setInput(type); }
	// External Interrupts
	// Warning: This will disable any previously enabled interrupt which is
	// routed to the same interupt line, e.g. PA3 will disable PB3.
	// This is a hardware limitation by the STM32 EXTI.
	inline static void enableExternalInterrupt()
	{
		// PA[x], x =  0 ..  3 maps to EXTICR[0]
		// PA[x], x =  4 ..  7 maps to EXTICR[1]
		// PA[x], x =  8 .. 11 maps to EXTICR[2]
		// PA[x], x = 12 .. 15 maps to EXTICR[3]
		// => bit3 and bit2 (mask 0x0c) specify the register
		// => bit1 and bit0 (mask 0x03) specify the bit position
		constexpr uint8_t index   = (pin & 0b1100) >> 2;
		constexpr uint8_t bit_pos = (pin & 0b0011) << 2;
		constexpr uint16_t syscfg_mask = (0b1111) << bit_pos;
		constexpr uint16_t syscfg_value = (port_nr & (0b1111)) << bit_pos;
		SYSCFG->EXTICR[index] = (SYSCFG->EXTICR[index] & ~syscfg_mask) | syscfg_value;
		EXTI->IMR |= mask;
	}
	inline static void disableExternalInterrupt() { EXTI->IMR &= ~mask; }
	inline static void enableExternalInterruptVector(const uint32_t priority)
	{
		NVIC_SetPriority(ExternalInterruptIRQ, priority);
		NVIC_EnableIRQ(ExternalInterruptIRQ);
	}
	inline static void disableExternalInterruptVector() { NVIC_DisableIRQ(ExternalInterruptIRQ); }
	inline static void setInputTrigger(const InputTrigger trigger)
	{
		switch (trigger)
		{
		case InputTrigger::RisingEdge:
			EXTI->RTSR |=  mask;
			EXTI->FTSR &= ~mask;
			break;
		case InputTrigger::FallingEdge:
			EXTI->RTSR &= ~mask;
			EXTI->FTSR |=  mask;
			break;
		case InputTrigger::BothEdges:
			EXTI->RTSR |=  mask;
			EXTI->FTSR |=  mask;
			break;
		}
	}
	inline static bool getExternalInterruptFlag() { return (EXTI->PR & mask); }
	inline static void acknowledgeExternalInterruptFlag() { EXTI->PR |= mask; }
	// GpioIO
	// start documentation inherited
	inline static Direction getDirection() {
		uint32_t mode = (GPIOB->MODER & mask2);
		if (mode == (i(Mode::Input) << pin * 2))
			return Direction::In;
		if (mode == (i(Mode::Output) << pin * 2))
			return Direction::Out;
		return Direction::Special;
	}
	// end documentation inherited
	inline static void lock() { PinSet::lock(); }
	inline static void disconnect() {
		setInput(InputType::Floating);
		GPIOB->AFR[af_id] &= ~af_mask;
	}

public:
#ifdef __DOXYGEN__
	/// @{
	/// Connect to any software peripheral
	using BitBang = GpioSignal;
	/// Connect to Tim2
	using Ch4 = GpioSignal;
	/// Connect to Sdio
	using Ck = GpioSignal;
	/// Connect to Quadspi
	using Clk = GpioSignal;
	/// Connect to Spi3
	using Mosi = GpioSignal;
	/// Connect to I2s3
	using Sd = GpioSignal;
	/// Connect to Sai1
	using SdA = GpioSignal;
	/// Connect to UsbOtgHs
	using UlpiD4 = GpioSignal;
	/// @}
#endif
	/// @cond
	template< Peripheral peripheral >
	struct BitBang { static void connect();
		static_assert(
			(peripheral == Peripheral::BitBang),
			"GpioB2::BitBang only connects to software drivers!");
	};
	template< Peripheral peripheral >
	struct Ch4 { static void connect();
		static_assert(
			(peripheral == Peripheral::Tim2),
			"GpioB2::Ch4 only connects to Tim2!");
	};
	template< Peripheral peripheral >
	struct Ck { static void connect();
		static_assert(
			(peripheral == Peripheral::Sdio),
			"GpioB2::Ck only connects to Sdio!");
	};
	template< Peripheral peripheral >
	struct Clk { static void connect();
		static_assert(
			(peripheral == Peripheral::Quadspi),
			"GpioB2::Clk only connects to Quadspi!");
	};
	template< Peripheral peripheral >
	struct Mosi { static void connect();
		static_assert(
			(peripheral == Peripheral::Spi3),
			"GpioB2::Mosi only connects to Spi3!");
	};
	template< Peripheral peripheral >
	struct Sd { static void connect();
		static_assert(
			(peripheral == Peripheral::I2s3),
			"GpioB2::Sd only connects to I2s3!");
	};
	template< Peripheral peripheral >
	struct SdA { static void connect();
		static_assert(
			(peripheral == Peripheral::Sai1),
			"GpioB2::SdA only connects to Sai1!");
	};
	template< Peripheral peripheral >
	struct UlpiD4 { static void connect();
		static_assert(
			(peripheral == Peripheral::UsbOtgHs),
			"GpioB2::UlpiD4 only connects to UsbOtgHs!");
	};
	/// @endcond
private:
	template< Peripheral peripheral >
	static constexpr int8_t AdcChannel = -1;
};

/// @cond
template<>
struct GpioB2::BitBang<Peripheral::BitBang>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::BitBang;
	static constexpr int af = -1;
	inline static void connect() {}
};
template<>
struct GpioB2::Ch4<Peripheral::Tim2>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ch4;
	static constexpr int af = 1;
	inline static void
	connect()
	{
		setAlternateFunction(1);
	}
};
template<>
struct GpioB2::Ck<Peripheral::Sdio>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Ck;
	static constexpr int af = 12;
	inline static void
	connect()
	{
		setAlternateFunction(12);
	}
};
template<>
struct GpioB2::Clk<Peripheral::Quadspi>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Clk;
	static constexpr int af = 9;
	inline static void
	connect()
	{
		setAlternateFunction(9);
	}
};
template<>
struct GpioB2::Mosi<Peripheral::Spi3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Mosi;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioB2::Sd<Peripheral::I2s3>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::Sd;
	static constexpr int af = 7;
	inline static void
	connect()
	{
		setAlternateFunction(7);
	}
};
template<>
struct GpioB2::SdA<Peripheral::Sai1>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::SdA;
	static constexpr int af = 6;
	inline static void
	connect()
	{
		setAlternateFunction(6);
	}
};
template<>
struct GpioB2::UlpiD4<Peripheral::UsbOtgHs>
{
	using Gpio = GpioB2;
	static constexpr Gpio::Signal Signal = Gpio::Signal::UlpiD4;
	static constexpr int af = 10;
	inline static void
	connect()
	{
		setAlternateFunction(10);
	}
};
/// @endcond

} // namespace platform

} // namespace modm

#endif // MODM_STM32_GPIO_PIN_B2_HPP