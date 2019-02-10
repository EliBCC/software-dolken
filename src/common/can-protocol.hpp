#pragma once

#include <modm/processing/protothread.hpp>
#include <common/board.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

enum class CanId : uint8_t {
    PressureCh1234,
    PressureCh5678,
    ValvePosCh1234,
    ValvePosCh5678,

    SetValvePosCh1234,
    SetValvePosCh5678,

};

class CanProtocolThread : public modm::pt::Protothread
{
public:
    bool run();

    void sendPressureCh1234(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);
    void sendPressureCh5678(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);
    void sendValvePosCh1234(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);
    void sendValvePosCh5678(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);
    void sendSetValvePosCh1234(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);
    void sendSetValvePosCh5678(modm::ShortTimestamp timestamp, uint16_t pressureData[4]);

protected:
    void onPressureCh1234(const modm::can::Message&) {};
    void onPressureCh5678(const modm::can::Message&) {};
    void onValvePosCh1234(const modm::can::Message&) {};
    void onValvePosCh5678(const modm::can::Message&) {};
    void onSetValvePosCh1234(const modm::can::Message&) {};
    void onSetValvePosCh5678(const modm::can::Message&) {};

private:
};