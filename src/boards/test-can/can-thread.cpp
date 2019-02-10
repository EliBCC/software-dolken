#include "globals.hpp"
#include <modm/debug/logger.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

void CanThread::onSetValvePosCh1234(const modm::can::Message& msg) {
	uint16_t timestamp = (msg.data[0] << 8) | (msg.data[1]);
	uint16_t* dest = actuatorThread.valvePos;
	dest[0] = ((msg.data[2] << 4) & 0xFF0) | ((msg.data[3] >> 4) & 0x00F);
	dest[1] = ((msg.data[3] << 8) & 0xF00) | ((msg.data[4]) & 0x0FF);
	dest[2] = ((msg.data[5] << 4) & 0xFF0) | ((msg.data[6] >> 4) & 0x00F);
	dest[3] = ((msg.data[6] << 8) & 0xF00) | ((msg.data[7]) & 0x0FF);
	MODM_LOG_INFO << "Actuator: received SetValvePos1234Msg from time " << timestamp;
}

void CanThread::onValvePosCh1234(const modm::can::Message& msg) {
	MODM_LOG_INFO << "Measurement: received ValvePos1234Msg";
	uint16_t timestamp = (msg.data[0] << 8) | (msg.data[1]);
	uint16_t* dest = actuatorThread.valvePos;
	dest[0] = ((msg.data[2] << 4) & 0xFF0) | ((msg.data[3] >> 4) & 0x00F);
	dest[1] = ((msg.data[3] << 8) & 0xF00) | ((msg.data[4]) & 0x0FF);
	dest[2] = ((msg.data[5] << 4) & 0xFF0) | ((msg.data[6] >> 4) & 0x00F);
	dest[3] = ((msg.data[6] << 8) & 0xF00) | ((msg.data[7]) & 0x0FF);
	MODM_LOG_INFO << "Measurement: received ValvePos1234Msg from time " << timestamp;
}
