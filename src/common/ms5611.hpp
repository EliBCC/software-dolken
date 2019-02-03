/*
* Written by Nicolai Dynweber Bruhn and Amar Shah
*/

#pragma once

#include <modm/processing/timer.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/processing/resumable.hpp>
#include <modm/platform.hpp>
#include <modm/architecture/interface/spi_device.hpp>
#include <modm/debug/logger.hpp>

// Set the log level
#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::WARNING

/**
 * @brief Namespace for non-template-specific items
 */
namespace ms5611
{
	/**
	 * @brief SPI commands for the MS5611 chip.
	 */
	enum class
	Command : uint8_t
	{
		Reset = 0x1E,
		ConvPressure = 0x40,
		ConvTemperature = 0x50,
		AdcRead = 0x00,
		PromRead = 0xA0
	};

	/**
	 * @brief Oversampling modes, OR with ConvPressure or ConvTemperature.
	 */
	enum class
	Oversampling : uint8_t
	{
		Osr256 = 0x0,
		Osr512 = 0x2,
		Osr1024 = 0x4,
		Osr2048 = 0x6,
		Osr4096 = 0x8
	};

	constexpr uint8_t oversamplingDelay(const Oversampling osr) {
		switch (osr) {
			case Oversampling::Osr256:
				return 2;
			case Oversampling::Osr512:
				return 3;
			case Oversampling::Osr1024:
				return 4;
			case Oversampling::Osr2048:
				return 6;
			case Oversampling::Osr4096:
			default:
				return 11;
		}
	}
};


template<typename SpiMaster, typename Cs>
class Ms5611 : public modm::SpiDevice<SpiMaster>, protected modm::NestedResumable<2>
{
public:
	/**
	 * @brief Constructs MS5611 Driver
	 */
	Ms5611() {}

	modm::ResumableResult<bool> initialize() {
		RF_BEGIN();
		this->attachConfigurationHandler([]() {
			SpiMaster::setDataMode(SpiMaster::DataMode::Mode0);
			SpiMaster::setDataOrder(SpiMaster::DataOrder::MsbFirst);
		});
		Cs::setOutput(modm::Gpio::High);

		RF_CALL(reset());

		promContents[0] = RF_CALL(readProm(0));
		promContents[1] = RF_CALL(readProm(1));
		promContents[2] = RF_CALL(readProm(2));
		promContents[3] = RF_CALL(readProm(3));
		promContents[4] = RF_CALL(readProm(4));
		promContents[5] = RF_CALL(readProm(5));
		promContents[6] = RF_CALL(readProm(6));
		promContents[7] = RF_CALL(readProm(7));

		RF_END_RETURN(validatePromCrc());
	}

	/**
	 * @returns value from the sensor's PROM at the given address.
	 * @param address must be between 0 and 7 inclusive.
	 */
	uint16_t getProm(uint8_t address) {
		if (address >= 8) return 0xFFFF;
		else return promContents[address];
	}

	modm::ResumableResult<void>
	startConversion(ms5611::Command adc, ms5611::Oversampling osr) {
		RF_BEGIN();

		txBuffer[0] = static_cast<uint8_t>(adc) | static_cast<uint8_t>(osr);

		RF_WAIT_UNTIL(adcReadTimeout.isExpired());
		RF_WAIT_UNTIL(this->acquireMaster());
		Cs::reset();

		RF_CALL(SpiMaster::transfer(txBuffer, rxBuffer, 1));

		if (this->releaseMaster()) {
			Cs::set();
		}

		adcReadTimeout.restart(ms5611::oversamplingDelay(osr));

		RF_END();
	}

	modm::ResumableResult<uint32_t>
	readConversionResult() {
		RF_BEGIN();

		txBuffer[0] = static_cast<uint8_t>(ms5611::Command::AdcRead);
		txBuffer[1] = 0;
		txBuffer[2] = 0;
		txBuffer[3] = 0;

		RF_WAIT_UNTIL(adcReadTimeout.isExpired());
		RF_WAIT_UNTIL(this->acquireMaster());
		Cs::reset();

		RF_CALL(SpiMaster::transfer(txBuffer, rxBuffer, 4));

		if (this->releaseMaster()) {
			Cs::set();
		}

		RF_END_RETURN(static_cast<uint32_t>((rxBuffer[1] << 16) | (rxBuffer[2] << 8) | rxBuffer[3]));
	}

	int32_t temperatureCentiCelsius(uint32_t temperatureRaw) {
		int32_t dT = temperatureRaw - (promContents[5] << 8);
		return 2000 + ((dT * promContents[6]) >> 23);
	}

private:
	modm::ResumableResult<void>
	reset() {
		RF_BEGIN();

		txBuffer[0] = static_cast<uint8_t>(ms5611::Command::Reset);

		RF_WAIT_UNTIL(this->acquireMaster());
		Cs::reset();

		RF_CALL(SpiMaster::transfer(txBuffer, nullptr, 1));

		if (this->releaseMaster()) {
			Cs::set();
		}

		adcReadTimeout.restart(4);

		RF_END();
	}

	modm::ResumableResult<uint16_t>
	readProm(uint8_t address) {
		RF_BEGIN();
		if (address >= 8) {
			RF_RETURN(0xFFFF);
		}
		txBuffer[0] = static_cast<uint8_t>(ms5611::Command::PromRead) | (address << 1);
		txBuffer[1] = 0;
		txBuffer[2] = 0;

		RF_WAIT_UNTIL(adcReadTimeout.isExpired());
		RF_WAIT_UNTIL(this->acquireMaster());
		Cs::reset();

		RF_CALL(SpiMaster::transfer(txBuffer, rxBuffer, 3));

		if (this->releaseMaster()) {
			Cs::set();
		}

		RF_END_RETURN(static_cast<uint16_t>((rxBuffer[1] << 8) | rxBuffer[2]));
	}

	/**
	 * From AN520: C-code example for MS56xx, MS57xx (except analog sensor), and MS58xx series pressure sensors
	 */
	uint16_t validatePromCrc() {
		int cnt;                                      // simple counter
		uint16_t n_rem;                               // crc reminder
		uint16_t crc_read;                            // original value of the crc
		uint8_t n_bit;
		n_rem = 0x00;
		crc_read=promContents[7];                     //save read CRC
		promContents[7]=(0xFF00 & (promContents[7])); //CRC byte is replaced by 0
		for (cnt = 0; cnt < 16; cnt++) {              // operation is performed on bytes
			// choose LSB or MSB
			if (cnt%2==1) n_rem ^= (unsigned short) ((promContents[cnt>>1]) & 0x00FF);
			else n_rem ^= (unsigned short) (promContents[cnt>>1]>>8);

			for (n_bit = 8; n_bit > 0; n_bit--) {
				if (n_rem & (0x8000)) {
					n_rem = (n_rem << 1) ^ 0x3000;
				}
				else {
					n_rem = (n_rem << 1);
				}
			}
		}
		n_rem=  (0x000F & (n_rem >> 12)); // // final 4-bit reminder is CRC code
		promContents[7]=crc_read;               // restore the crc_read to its original place
		return (n_rem ^ 0x00);
	}

	uint8_t txBuffer[3];
	uint8_t rxBuffer[3];
	uint16_t promContents[8];

    modm::ShortTimeout adcReadTimeout;
};
