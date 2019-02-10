#include "globals.hpp"
#include <modm/debug/logger.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::DEBUG

ActuatorThread::ActuatorThread() : timer(100) {};

bool ActuatorThread::run() {
	PT_BEGIN();
		timer.restart(100);
		while(1) {
			PT_WAIT_UNTIL(timer.execute());
			canThread.sendValvePosCh1234(modm::Clock::nowShort(), this->valvePos);
		}
	PT_END();
}