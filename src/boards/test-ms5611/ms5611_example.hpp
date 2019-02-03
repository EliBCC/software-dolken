/*
* Written by Nicolai Dynweber Bruhn
*/

#include <modm/processing/protothread.hpp>
#include <modm/processing/timer/timeout.hpp>
#include <common/ms5611.hpp>
#include <modm/math/filter.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

class Ms5611ExampleThread : public modm::pt::Protothread, private modm::NestedResumable<2>
{
public:
	inline Ms5611ExampleThread()
	{};

	inline bool
    run()
	{
        PT_BEGIN();

		if (PT_CALL(ms5611.initialize())) {
			MODM_LOG_INFO << "MS5611 initialized successfully" << modm::endl;
		}
		else {
			MODM_LOG_INFO << "MS5611 ROM CRC validation failed" << modm::endl;
		}

		i = 0;
        while (true)
        {
			timeout.restart(10);

			PT_CALL(ms5611.startConversion(ms5611::Command::ConvPressure, ms5611::Oversampling::Osr4096));
			pressureAvg.update(PT_CALL(ms5611.readConversionResult())));
			PT_CALL(ms5611.startConversion(ms5611::Command::ConvTemperature, ms5611::Oversampling::Osr256));
			temperature = PT_CALL(ms5611.readConversionResult()));

			MODM_LOG_INFO.printf("MS5611 Pressure (raw, 16x rolling average): %lu, Temperature (C): %ld", pressureAvg.getValue(), ms5611.temperatureCentiCelsius(temperature));
        }

        PT_END();
    }
private:

    modm::ShortTimeout timeout;

	Ms5611<modm::platform::SpiMaster1, modm::platform::GpioA0> ms5611;

	modm::filter::MovingAverage<uint32_t, 16> pressureAvg;
	uint32_t temperature;
	int i;
};


int ads7828ExampleBlocking();