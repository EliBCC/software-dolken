#pragma once
#include <common/can-protocol.hpp>
#include <modm/processing/timer/periodic_timer.hpp>

class MeasurementThread : modm::pt::Protothread {
public:
	bool run();
	uint16_t measuredValvePos[8];
private:
	uint16_t targetValvePos[8];
	bool measuredValvePosUpdated = true;
	modm::ShortTimeout timer;
};