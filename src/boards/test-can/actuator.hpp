#pragma once
#include <modm/processing/timer/periodic_timer.hpp>

class ActuatorThread : modm::pt::Protothread {
public:
	ActuatorThread();
	bool run();
	uint16_t valvePos[8];
private:
	modm::ShortPeriodicTimer timer;
};