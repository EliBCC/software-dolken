/*
* Written by Nicolai Dynweber Bruhn
*/

#include <modm/processing/protothread.hpp>
#include <modm/processing/timer/periodic_timer.hpp>
#include <common/board.hpp>
#include <common/ads7828.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::DISABLED

class ColdflowThread : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
	inline ColdflowThread() : adc(ads7828::GetAddress(true, false)), timer(20)
	{};

	inline bool
    run()
	{
        PT_BEGIN();

        while (true)
        {
			PT_WAIT_UNTIL(timer.execute());

			MODM_LOG_INFO << "Coldflow: Reading sensor" << modm::endl;

			adcReadings[0] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single0));
			adcReadings[1] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single1));
			adcReadings[2] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single2));
			adcReadings[3] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single3));
			adcReadings[4] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single4));
			adcReadings[5] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single5));
			adcReadings[6] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single6));
			adcReadings[7] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single7));

			MODM_LOG_INFO << "Coldflow: ";
			for (i = 0; i < 8; i++)
			{
				MODM_LOG_INFO.printf(" %4d", adcReadings[i]);
			}
			MODM_LOG_INFO << modm::endl;

			for (i = 0; i < 6; i++) {
				buffer[i * 3 + 3] = (adcReadings[i + 2] >> 8) & 0xFF;
				buffer[i * 3 + 4] = (adcReadings[i + 2]) & 0xFF;
			}
			for (i = 0; i < sizeof(buffer); i++) {
				MODM_LOG_INFO.printf("0x%x ", buffer[i]);
			}
			MODM_LOG_INFO << modm::endl;
			SerialDebug::writeBlocking(buffer, sizeof(buffer));
        }

        PT_END();
    }
private:
	//The sensor is on I2C address 73 on the sensor board
	Ads7828<modm::platform::I2cMaster1> adc;
	//For delays in the thread
    modm::ShortPeriodicTimer timer;
	//ADC readout values.
	uint16_t adcReadings[8];
	// Loop index
	unsigned int i;
	// Packet buffer
	uint8_t buffer[24] = {
		0xFF, 0x01,
		0x16, 0x0, 0x0,
		0x17, 0x0, 0x0,
		0x18, 0x0, 0x0,
		0x19, 0x0, 0x0,
		0x20, 0x0, 0x0,
		0x21, 0x0, 0x0,
		0x01, 0xFF, 0xFF, 0xFF
	};
};

