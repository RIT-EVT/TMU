#ifndef TMU_MAX31855_H
#define TMU_MAX31855_H

#include <EVT/io/SPI.hpp>
#include <cstdint>
#include <utility>

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

    /*
     * The status of the MAX31855 after starting a temperature read.
     *
     * NO_ERROR: No error
     * SCV_FAULT: Short to VCC
     * SCG_FAULT: Short to GND
     * OC_FAULT: Open Circuit
     * MAX31855_ERROR: Error reading from the MAX31855
     */
    enum class MaxStatus {
        NO_ERROR = 0,
        SCV_FAULT = 1,
        SCG_FAULT = 2,
        OC_FAULT = 3,
        MAX31855_ERROR = 4
    };

    /**
     * Reads the temperature from the MAX31855 and updates the temp variable passed in.
     *
     * @param[in] temp The temperature to update using the readTemp method.
     *
     * @return a MaxStatus enum indicating the status of the read.
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
