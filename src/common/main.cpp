/*
 * Copyright (c) 2013, Kevin Läufer
 * Copyright (c) 2013-2018, Niklas Hauser
 * Copyright (c) 2014, Sascha Schade
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------


#include <common/board.hpp>
#include <common/config.hpp>
#include <modm/processing/timer.hpp>
#include <modm/debug/logger.hpp>

using namespace modm::platform;

modm::IODeviceWrapper<Usart2, modm::IOBuffer::BlockIfFull> loggerDevice;
modm::log::Logger modm::log::info(loggerDevice);

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO


static void displayMessage(const modm::can::Message& message) {
	static uint32_t receiveCounter = 0;
	receiveCounter++;

	MODM_LOG_INFO<< "id  =" << message.getIdentifier();
	if (message.isExtended()) {
		MODM_LOG_INFO<< " extended";
	}
	else {
		MODM_LOG_INFO<< " standard";
	}
	if (message.isRemoteTransmitRequest()) {
		MODM_LOG_INFO<< ", rtr";
	}
	MODM_LOG_INFO<< modm::endl;

	MODM_LOG_INFO<< "dlc =" << message.getLength() << modm::endl;
	if (!message.isRemoteTransmitRequest())
	{
		MODM_LOG_INFO << "data=";
		for (uint32_t i = 0; i < message.getLength(); ++i) {
			MODM_LOG_INFO<< modm::hex << message.data[i] << modm::ascii << ' ';
		}
		MODM_LOG_INFO<< modm::endl;
	}
	MODM_LOG_INFO<< "# received=" << receiveCounter << modm::endl;
}

int main(void) {
	Board::initialize();

	MODM_LOG_INFO << "MainBoard Test Program" << modm::endl;
	MODM_LOG_DEBUG << "debug" << modm::endl;
    MODM_LOG_INFO << "info" << modm::endl;

	// Set up to receive every message
	MODM_LOG_INFO << "Setting up Filter for Can ..." << modm::endl;
	CanFilter::setFilter(0, CanFilter::FIFO0,
		CanFilter::ExtendedIdentifier(0),
		CanFilter::ExtendedFilterMask(0));


	// Send a message
	//MODM_LOG_INFO << "Sending message on Can ..." << modm::endl;
	//modm::can::Message msg1(1, 1);
	//msg1.setExtended(true);
	//msg1.data[0] = 0x11;
	//Can1::sendMessage(msg1);

	modm::ShortPeriodicTimer pTimer(100);

	while (1)
	{
		if (Can1::isMessageAvailable()) {
			MODM_LOG_INFO << "Can1: Message is available..." << modm::endl;
			modm::can::Message message;
			Can1::getMessage(message);
			displayMessage(message);
			Board::LedGreen::toggle();
			modm::delayMilliseconds(125);
			Board::LedGreen::toggle();
		}

		if (pTimer.execute()) {
			static uint8_t idx = 0;
			modm::can::Message msg1(1, 1);
			msg1.setExtended(true);
			msg1.data[0] = idx;
			Can1::sendMessage(msg1);
			++idx;
			Board::LedGreen::toggle();
			modm::delayMilliseconds(125);
			Board::LedGreen::toggle();
		}
	}
	return 0;
}
