#include <EVT/io/SPI.hpp>
#include <cstdint>

using namespace EVT::core::IO;

namespace DEV {

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
     * Returns the temp in centicelsius
     *
     * @return The temp
     */
    uint16_t readTemp();

private:
    /** The SPI interface to read from */
    SPI& spi;
    /** The SPI device number for this device */
    uint8_t device;
};

}

#ifndef TMU_MAX31855_H

    #define TMU_MAX31855_H
#endif//TMU_MAX31855_H
