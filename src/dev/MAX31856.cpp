#include "dev/MAX31856.h"
#include <EVT/io/SPI.hpp>

using namespace EVT::core::IO;
namespace MAX31856 {

MAX31856::MAX31856(SPI& SPI) : spi(SPI) {}
int32_t MAX31856::readTemp() {
    uint8_t bytes[3];
    int32_t tempValue = 0; // only use 19 bits of the 32 bit

    spi.readReg(0, 12, bytes, 2);
    tempValue |= bytes[0];
//    spi.readReg(0, 13, bytes, 2);
//    tempValue = (tempValue << 8) | bytes[0];
//    spi.readReg(0, 14, bytes, 2);
//    tempValue = (tempValue << 8) | bytes[0];
//
//    tempValue = tempValue << 8; // convert to 32 bit

    return (tempValue);
}

}