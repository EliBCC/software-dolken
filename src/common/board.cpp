#include "board.hpp"
#include <modm/architecture/interface/clock.hpp>
#include <modm/debug/logger.hpp>


modm::IODeviceWrapper<SerialDebug, modm::IOBuffer::BlockIfFull> debugLogger;
modm::log::Logger modm::log::debug(debugLogger);
modm::log::Logger modm::log::info(debugLogger);
modm::log::Logger modm::log::warning(debugLogger);
modm::log::Logger modm::log::error(debugLogger);

bool ClockConfiguration::enable() {
	// modm::platform::ClockControl can be accessed as only ClockControl
	using namespace modm::platform;
	//ClockControl::enableExternalCrystal(); // 26 MHz
	ClockControl::enableInternalClock(); //16 MHz
	ClockControl::enablePll(
		ClockControl::PllSource::InternalClock,
		16,      // 16MHz / N=16 -> 1MHz   !!! Must be 1 MHz for PLLSAI !!!
		336,    // 1MHz * M=336 -> 336MHz
		2,      // 336MHz / P=2 -> 168MHz = F_cpu
		7       // 336MHz / Q=7 -> 48MHz (value ignored! PLLSAI generates 48MHz for F_usb)
	);

	ClockControl::setFlashLatency(Frequency);
	ClockControl::setApb1Prescaler(ClockControl::Apb1Prescaler::Div4);
	ClockControl::setApb2Prescaler(ClockControl::Apb2Prescaler::Div2);

	ClockControl::enableSystemClock(ClockControl::SystemClockSource::Pll);
	// update clock frequencies
	modm::clock::fcpu     = Frequency;
	modm::clock::fcpu_kHz = Frequency / 1000;
	modm::clock::fcpu_MHz = Frequency / 1000000;
	modm::clock::ns_per_loop = ::round(3000.f / (Frequency / 1000000));

	return true;
}

void initCommon() {
	using namespace modm::platform;

	ClockConfiguration::enable();
	modm::cortex::SysTickTimer::initialize<ClockConfiguration>();

	LedD2::setOutput(modm::Gpio::Low);
	LedD3::setOutput(modm::Gpio::High);

	SerialDebug::connect<modm::platform::GpioA9::Tx>();
	SerialDebug::initialize<ClockConfiguration, 115200>();

	I2cMaster1::connect<GpioB7::Sda, GpioB6::Scl>(I2cMaster1::PullUps::Internal);
	I2cMaster1::initialize<ClockConfiguration, I2cMaster1::Baudrate::Standard>();
}