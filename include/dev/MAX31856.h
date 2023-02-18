#ifndef TMU_MAX31856_H
#define TMU_MAX31856_H

#endif//TMU_MAX31856_H

#include "EVT/io/SPI.hpp"
#include "cstdint"

using namespace EVT::core::IO;
namespace MAX31856 {

class MAX31856 {
public:
    explicit MAX31856(SPI& spi);
    int32_t readTemp();
private:
    SPI& spi;
};

}