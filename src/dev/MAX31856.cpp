#include "dev/MAX31856.h"
#include <EVT/io/SPI.hpp>

using namespace EVT::core::IO;
namespace MAX31856 {

MAX31856::MAX31856(SPI& SPI) : spi(SPI) {}
int32_t MAX31856::readTemp(uint16_t reg) {
    return 0;
}

}