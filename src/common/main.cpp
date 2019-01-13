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
#include <mainboard/mainboard.hpp>

using namespace modm::platform;

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

static void blinkTest() {
	Board::LedOrangeD2::toggle();
	
	for (int i = 0; i < 10; i++) {
		Board::LedOrangeD2::toggle();
		Board::LedOrangeD3::toggle();
		modm::delayMilliseconds(50);
	}

	Board::LedOrangeD2::toggle();
}

static void printCanStatus() {
	MODM_LOG_INFO << "Can1 state: " << Can1::getBusState() << modm::endl;
	MODM_LOG_INFO << "Can1 message available: " << Can1::isMessageAvailable() << modm::endl;
	MODM_LOG_INFO << "Can1 ready to send: " << Can1::isReadyToSend() << modm::endl;
	MODM_LOG_INFO << "Can1 receive errors: " << Can1::getReceiveErrorCounter() << modm::endl;
	MODM_LOG_INFO << "Can1 transmit errors: " << Can1::getTransmitErrorCounter() << modm::endl;
}

int main(void) {
	Board::initialize();

	MODM_LOG_INFO << "MainBoard CAN Test Program" << modm::endl;
	MODM_LOG_DEBUG << "debug" << modm::endl;
    MODM_LOG_INFO << "info" << modm::endl;
    MODM_LOG_WARNING << "warning" << modm::endl;
	MODM_LOG_ERROR << "error" << modm::endl;

	blinkTest();

	MODM_LOG_INFO << "1" << modm::endl;
	Can1::connect<GpioA11::Rx, GpioA12::Tx>(Gpio::InputType::PullUp);
	MODM_LOG_INFO << "2" << modm::endl;
	Can1::initialize<Board::systemClock, Can1::Bitrate::kBps125>(9);
	MODM_LOG_INFO << "3" << modm::endl;
	Can1::setAutomaticRetransmission(false);
	MODM_LOG_INFO << "4" << modm::endl;

	// Set up to receive every message
	MODM_LOG_INFO << "Setting up Filter for Can1 ..." << modm::endl;
	CanFilter::setFilter(0, CanFilter::FIFO0,
		CanFilter::ExtendedIdentifier(0),
		CanFilter::ExtendedFilterMask(0));

	// // Send a message
	// MODM_LOG_INFO << "Sending message on Can ..." << modm::endl;
	// modm::can::Message msg1(1, 1);
	// msg1.setExtended(true);
	// msg1.data[0] = 0x11;
	// Can1::sendMessage(msg1);

	modm::ShortPeriodicTimer pTimer(1000);

	while (1) {
		if (Can1::isMessageAvailable()) {
			MODM_LOG_INFO << "Can1: Message is available..." << modm::endl;
			modm::can::Message message;
			Can1::getMessage(message);
			displayMessage(message);
			Board::LedOrangeD2::toggle();
			modm::delayMilliseconds(125);
			Board::LedOrangeD2::toggle();
		}

		if (pTimer.execute()) {
			printCanStatus();
			// MODM_LOG_INFO << "Can1: Sending message..." << modm::endl;
			// static uint8_t idx = 0;
			// modm::can::Message msg1(2, 1);
			// msg1.setExtended(true);
			// msg1.data[0] = idx;
			// Can1::sendMessage(msg1);
			// ++idx;
			Board::LedOrangeD3::toggle();
			modm::delayMilliseconds(125);
			Board::LedOrangeD3::toggle();
		}
	}
	return 0;
}
