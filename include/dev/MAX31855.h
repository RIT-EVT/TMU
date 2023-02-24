#ifndef TMU_MAX31856_H
#define TMU_MAX31856_H

#endif//TMU_MAX31856_H

#include "EVT/io/SPI.hpp"
#include "cstdint"

using namespace EVT::core::IO;
namespace MAX31855 {

/**
 * Handles reading temp data from the MAX31855 ADC
 */
class MAX31855 {
public:
    /**
     * Creates a new MAX31855 which will read a raw ADC temp.
     *
     * @param[in] spi The SPI to use to communicate with the MAX31855
     */
    explicit MAX31855(SPI& spi);
    /**
     * Returns the temp in celsius
     *
     * @return The temp
     */
    int16_t readTemp();
private:
    /** The SPI interface to read from */
    SPI& spi;
};

}
