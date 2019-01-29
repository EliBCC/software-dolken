#include <modm/architecture/interface/delay.hpp>
#include <modm/debug/logger.hpp>

#include <common/board.hpp>
#include <common/blink_thread.hpp>
#include <common/w25m.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

class FlashExampleThread : public modm::pt::Protothread, private modm::NestedResumable<6>
{
public:
	inline FlashExampleThread()
	{};

	inline bool
    run()
	{
        PT_BEGIN();
		MODM_LOG_INFO << "Initializing Flash" << modm::endl;
		PT_CALL(flash.initialize());
		MODM_LOG_INFO << "Reading Flash ID" << modm::endl;
		flashId = PT_CALL(flash.readId());
		MODM_LOG_INFO.printf("id: 0x%x, type: 0x%x, manufacturer: 0x%x\n", flashId.deviceId, flashId.deviceType, flashId.manufacturerId);
		while(1) {
			PT_YIELD();
		}
        PT_END();
    }


private:
	//For delays in the thread
    modm::ShortTimeout timeout;

	//The sensor is on I2C address 73 on the sensor board
	W25m<modm::platform::SpiMaster2, modm::platform::GpioA1, 262144, 0> flash;

	//Flash JEDEC ID read from chip
	decltype(flash)::JedecId flashId;

	// Loop index
	int i;
};

int main(void) {
	initCommon();

	modm::platform::GpioA1::setOutput(modm::Gpio::High);
	modm::platform::SpiMaster2::connect<modm::platform::GpioC1::Mosi,
	                                    modm::platform::GpioC2::Miso,
	                                    modm::platform::GpioB10::Sck>();

	modm::platform::SpiMaster2::initialize<ClockConfiguration, 656250>();


	MODM_LOG_INFO << "======Flash test======" << modm::endl;
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

	BlinkThread blinkThread;
	FlashExampleThread flashExampleThread;
	while (1) {
		blinkThread.run();
		flashExampleThread.run();
	}
	return 0;
}