#ifndef TMU_MAX31856_H
#define TMU_MAX31856_H

#endif//TMU_MAX31856_H

#include "EVT/io/SPI.hpp"
#include "cstdint"

using namespace EVT::core::IO;
namespace MAX31855 {

class MAX31855 {
public:
    explicit MAX31855(SPI& spi);
    int16_t readTemp();
private:
    SPI& spi;
};

}