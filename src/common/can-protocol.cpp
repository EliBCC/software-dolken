#include "can-protocol.hpp"
#include <modm/platform.hpp>
#include <modm/debug/logger.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::DEBUG

using Can = modm::platform::Can1;

bool CanProtocolThread::run() {
	if (Can::isMessageAvailable()) {
		modm::can::Message msg;
		Can::getMessage(msg);
		switch (static_cast<CanId>(msg.getIdentifier())) {
			case CanId::PressureCh1234:
				onPressureCh1234(msg);
				break;
			case CanId::PressureCh5678:
				onPressureCh5678(msg);
				break;
			case CanId::ValvePosCh1234:
				onValvePosCh1234(msg);
				break;
			case CanId::ValvePosCh5678:
				onValvePosCh5678(msg);
				break;
			default:
				break;
		}
	}
	return true;
}

void CanProtocolThread::sendValvePosCh1234(modm::ShortTimestamp timestamp, uint16_t pressureData[4]) {
	modm::can::Message msg;
	msg.setIdentifier(static_cast<uint8_t>(CanId::SetValvePosCh1234));
	msg.setLength(8);
	msg.data[0] = (timestamp.getTime() >> 8) & 0xFF;
	msg.data[1] = (timestamp.getTime()) & 0xFF;

	msg.data[2] = (pressureData[0] >> 4) & 0xFF;
	msg.data[3] = ((pressureData[0] << 4) & 0xF0) | ((pressureData[1] >> 8) & 0x0F);
	msg.data[4] = (pressureData[1] & 0xFF);

	msg.data[5] = (pressureData[2] >> 4) & 0xFF;
	msg.data[6] = ((pressureData[2] << 4) & 0xF0) | ((pressureData[3] >> 8) & 0x0F);
	msg.data[7] = (pressureData[3] & 0xFF);
	Can::sendMessage(msg);
	MODM_LOG_DEBUG << "sent valvepos" << modm::endl;
};

void CanProtocolThread::sendSetValvePosCh1234(modm::ShortTimestamp timestamp, uint16_t pressureData[4]) {
	modm::can::Message msg;
	msg.setIdentifier(static_cast<uint8_t>(CanId::SetValvePosCh1234));
	msg.setLength(8);
	msg.data[0] = (timestamp.getTime() >> 8) & 0xFF;
	msg.data[1] = (timestamp.getTime()) & 0xFF;

	msg.data[2] = (pressureData[0] >> 4) & 0xFF;
	msg.data[3] = ((pressureData[0] << 4) & 0xF0) | ((pressureData[1] >> 8) & 0x0F);
	msg.data[4] = (pressureData[1] & 0xFF);

	msg.data[5] = (pressureData[2] >> 4) & 0xFF;
	msg.data[6] = ((pressureData[2] << 4) & 0xF0) | ((pressureData[3] >> 8) & 0x0F);
	msg.data[7] = (pressureData[3] & 0xFF);
	Can::sendMessage(msg);
};