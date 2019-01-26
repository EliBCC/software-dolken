/*
* Written by Nicolai Dynweber Bruhn
*/

#include <modm/processing/protothread.hpp>
#include <modm/processing/timer/timeout.hpp>
#include <common/board.hpp>
#include <common/ads7828.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

class Ads7828ExampleThread : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
	inline Ads7828ExampleThread() : adc(ads7828::GetAddress(true, false))
	{};

	inline bool
    run()
	{
        PT_BEGIN();

		while (true)
		{
			timeout.restart(1000);
			MODM_LOG_INFO << "ADS7828 Example: Pinging ADC" << modm::endl;
			if (!PT_CALL(adc.ping())) {
				MODM_LOG_INFO << "ADS7828 Example: No response" << modm::endl;
			}
			else {
				MODM_LOG_INFO << "ADS7828 Example: Ping received" << modm::endl;
				break;
			}
			PT_WAIT_UNTIL(timeout.isExpired());
		}
        while (true)
        {
			timeout.restart(1000);
			MODM_LOG_INFO << "ADS7828 Example: Reading sensor" << modm::endl;

			adcReadings[0] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single0));
			adcReadings[1] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single1));
			adcReadings[2] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single2));
			adcReadings[3] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single3));
			adcReadings[4] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single4));
			adcReadings[5] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single5));
			adcReadings[6] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single6));
			adcReadings[7] = PT_CALL(adc.readChannel(ads7828::InputChannel::Single7));

			MODM_LOG_INFO << "ADS7828 Example: ";
			for (i = 0; i < 8; i++)
			{
				MODM_LOG_INFO.printf(" %4d", adcReadings[i]);
			}
			MODM_LOG_INFO << modm::endl;

			PT_WAIT_UNTIL(timeout.isExpired());
        }
        PT_END();
    }
private:
	//For delays in the thread
    modm::ShortTimeout timeout;
	//The sensor is on I2C address 73 on the sensor board
	Ads7828<modm::platform::I2cMaster1> adc;
	//ADC readout values.
	uint16_t adcReadings[8];
	// Loop index
	int i;
};


int ads7828ExampleBlocking();