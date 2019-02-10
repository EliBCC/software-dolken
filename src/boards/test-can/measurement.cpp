#include "measurement.hpp"
#include <modm/debug/logger.hpp>

#include "globals.hpp"

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO


bool MeasurementThread::run() {
	PT_BEGIN();
		canThread.sendSetValvePosCh1234(modm::Clock::nowShort(), this->targetValvePos);
		while(1) {
			timer.restart(1000);
			PT_WAIT_UNTIL(timer.execute());
			for(int i = 0; i < 4; i++) {
				this->targetValvePos[i] = this->measuredValvePos[i] + 1;
			}
			canThread.sendSetValvePosCh1234(modm::Clock::nowShort(), this->targetValvePos);
		}
	PT_END();
}