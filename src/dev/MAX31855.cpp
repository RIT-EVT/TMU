#include "dev/MAX31855.h"
#include <EVT/io/SPI.hpp>

using namespace EVT::core::IO;
namespace MAX31855 {

MAX31855::MAX31855(SPI& SPI) : spi(SPI) {}
int16_t MAX31855::readTemp() {
    int16_t temp;
    const int length = 4;
    uint8_t bytes[length];
    spi.startTransmission(0);
    spi.read(bytes, length);
    spi.endTransmission(0);
    temp = (((uint16_t)bytes[0]) << 8) | bytes[1];
    temp = temp >> 2; // lose last 2 bits as they aren't in the 14 bit temp
    temp = (temp>>2)*100 + (temp&0x03)*25; // convert last 2 digits to allow for decimal place
    return temp;
}

}
