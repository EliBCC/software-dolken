/*
* Written by Nicolai Dynweber Bruhn and Amar Shah
*/

#pragma once


#include <modm/processing/timer.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/processing/resumable.hpp>
#include <modm/architecture/interface/i2c_device.hpp>
#include <modm/debug/logger.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::DISABLED

/**
 * @brief Namespace for non-template-specific items
 */
namespace ads7828
{
    /**
     * @brief Enum representing inputs to the ADC
     * SingleX measures the voltage CHX, while DiffXY measures the
     * voltage difference CHX - CHY.
     */
    enum class
    InputChannel : uint8_t
    {
        Diff01 = 0b00000000,
        Diff23 = 0b00010000,
        Diff45 = 0b00100000,
        Diff67 = 0b00110000,
        Diff10 = 0b01000000,
        Diff32 = 0b01010000,
        Diff54 = 0b01100000,
        Diff76 = 0b01110000,
        Single0 = 0b10000000,
	    Single1	= 0b11000000,
		Single2 = 0b10010000,
		Single3	= 0b11010000,
		Single4 = 0b10100000,
		Single5 = 0b11100000,
		Single6	= 0b10110000,
		Single7	= 0b11110000
    };

    /**
     * @brief Returns the I2C address for the chip given configuration of pins A0 and A1.
     * @param A0 false for low, true for high
     * @param A1 false for low, true for high
     * @return I2C address in the range 72 to 75 inclusive.
     * For sensor board: A0 ground, A1 high
     */
    constexpr uint8_t GetAddress(bool A0, bool A1)
    {
        return 0b1001000 | ((A0) ? 0b01 : 0b0) | ((A1) ? 0b10 : 0b0);
    }

    /**
     * @brief Hardcoded settings for ADC, internal reference off and ADC on.
     */
    constexpr uint8_t CONTROL_PARAMETERS_BYTE = 0b00000100;
};


template < typename I2cMaster>
class Ads7828 : public modm::I2cDevice<I2cMaster, 2>
{
public:
    /**
     * @brief Constructs ADS7828 driver instance
     * @param address should be between 72 and 75 inclusive, depending on
     * configuration of pins A0 and A1 on the chip.
     */
    Ads7828(uint8_t address): modm::I2cDevice<I2cMaster, 2>(address)
    {}

    /**
     * @brief Resumable method to take an ADC reading.
     * @param channel may be single-ended or differential,
     * see @ref ads7827::inputChannel
     * @returns 12-bit ADC value, or 0xFFFF in case of error.
     */
    modm::ResumableResult<uint16_t> readChannel(ads7828::InputChannel channel)
    {
        RF_BEGIN();

        buffer[0] = ads7828::CONTROL_PARAMETERS_BYTE | (uint8_t)channel;

        this->transaction.configureWrite(buffer, 1);
        MODM_LOG_INFO.printf("ADS7828E: starting write transaction\n");
        if(not RF_CALL(this->runTransaction()))
        {
            MODM_LOG_ERROR.printf("ADS7828E: write transaction error\n");
            RF_RETURN(0xFFFF);
        }

        this->transaction.configureRead(buffer, 2);
        MODM_LOG_INFO.printf("ADS7828E: starting read transaction\n");
        if(not RF_CALL(this->runTransaction()))
        {
            MODM_LOG_ERROR.printf("ADS7828E: read transaction error\n");
            RF_RETURN(0xFFFF);
        }

        RF_END_RETURN(static_cast<uint16_t>((buffer[0] << 8) + buffer[1]));
    }

private:
    uint8_t buffer[2];
};
