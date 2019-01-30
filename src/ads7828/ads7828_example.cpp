#include "ads7828_example.hpp"
#include <common/ads7828.hpp>
#include <common/board.hpp>
#include <modm/debug/logger.hpp>
#include <modm/platform/i2c/i2c_master_1.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

int ads7828TestBlocking()
{
	uint8_t i2cAddr = ads7828::GetAddress(true, false);
	Ads7828<modm::platform::I2cMaster1> adc(i2cAddr);
	int i = 0;
	while(true)
	{
		MODM_LOG_INFO << i++ << modm::endl;
		MODM_LOG_INFO.printf("I2c address: %d (0x%x)\n", i2cAddr, i2cAddr);
		MODM_LOG_INFO << "Ping response: " << RF_CALL_BLOCKING(adc.ping()) << modm::endl;

		uint16_t val;
		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single0));
		MODM_LOG_INFO.printf("CH0: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single1));
		MODM_LOG_INFO.printf("CH1: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single2));
		MODM_LOG_INFO.printf("CH2: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single3));
		MODM_LOG_INFO.printf("CH3: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single4));
		MODM_LOG_INFO.printf("CH4: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single5));
		MODM_LOG_INFO.printf("CH5: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single6));
		MODM_LOG_INFO.printf("CH6: %d\n", val);

		val = RF_CALL_BLOCKING(adc.readChannel(ads7828::InputChannel::Single7));
		MODM_LOG_INFO.printf("CH7: %d\n", val);

		LedD3::reset();
		modm::delayMilliseconds(50);

		LedD3::set();
		modm::delayMilliseconds(50);

		modm::delayMilliseconds(1000);
	}
	return 0;
}