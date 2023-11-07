#ifndef TMU_MAX31855_H
#define TMU_MAX31855_H

#include <EVT/io/SPI.hpp>
#include <cstdint>

using namespace EVT::core::IO;

namespace TMU::DEV {

/**
 * Handles reading temp data from the MAX31855 ADC
 * https://www.analog.com/media/en/technical-documentation/data-sheets/max31855.pdf
 */
class MAX31855 {
public:
    /**
     * Creates a new MAX31855 which will read a raw ADC temp.
     *
     * @param[in] spi The SPI to use to communicate with the MAX31855
     * @param[in] device The device we are communicating with
     */
    explicit MAX31855(SPI& spi, uint8_t device);

    /**
     * @enum MaxStatus
     * @brief Status codes for MAX31855 temperature sensor operations.
     *
     * This enum represents the possible status codes that can be returned by
     * the MAX31855 temperature sensor operations.
     */
    enum MaxStatus {
            OK = 0,         ///< No error
            SCV_FAULT = 1,  ///< Short to VCC
            SCG_FAULT = 2,  ///< Short to GND
            OC_FAULT = 3,   ///< Open Circuit
            READ_ERROR = 4, ///< Error reading from the MAX31855
            SPI_ERROR = 5   ///< Error starting SPI Read
    };


    /**
     * Reads the temperature from the MAX31855 and updates the temp variable passed in.
     * Returns a MaxStatus to indicate if there was any errors.
     *
     * @param[in] temp The temperature to update using the readTemp method.
     * @param[out] temp The temperature reported by the IC.
     */
    MAX31855::MaxStatus readTemp(uint16_t& temp);

private:
    /** The SPI interface to read from */
    SPI& spi;
    /** The SPI device number for this device */
    uint8_t device;
};

}// namespace TMU::DEV

#endif//TMU_MAX31855_H
