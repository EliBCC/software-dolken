/*
* Written by Nicolai Dynweber Bruhn
*/

#include <modm/processing/protothread.hpp>
#include <modm/processing/timer/timeout.hpp>
#include <modm/debug/logger.hpp>
#include <common/board.hpp>
#include <common/pca9685.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

class Pca9685ExampleThread : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
	inline Pca9685ExampleThread() : pwm(0x40)
	{};

	inline bool
    run()
	{
        PT_BEGIN();

		while (true)
		{
			timeout.restart(1000);
			MODM_LOG_INFO << "PCA9685 Example: Pinging PWM driver" << modm::endl;
			if (!PT_CALL(pwm.ping())) {
				MODM_LOG_INFO << "PCA9685 Example: No response" << modm::endl;
			}
			else {
				MODM_LOG_INFO << "PCA9685 Example: Ping received" << modm::endl;
				break;
			}
			PT_WAIT_UNTIL(timeout.isExpired());
		}
        while (true)
        {
			timeout.restart(1000);
			PT_WAIT_UNTIL(timeout.isExpired());
        }
        PT_END();
    }
private:
	//For delays in the thread
    modm::ShortTimeout timeout;
	//The sensor is on I2C address 73 on the sensor board
	Pca9685<modm::platform::I2cMaster1> pwm;
	//ADC readout values.
	uint16_t adcReadings[8];
	// Loop index
	int i;
};
