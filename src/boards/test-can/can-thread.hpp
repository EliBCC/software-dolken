#pragma once
#include <common/can-protocol.hpp>

class CanThread : public CanProtocolThread {
public:
	void onSetValvePosCh1234(const modm::can::Message& msg);
	void onValvePosCh1234(const modm::can::Message& msg);
};