#include <EVT/io/SPI.hpp>
#include <dev/MAX31855.hpp>

using namespace EVT::core::IO;

namespace TMU::DEV {

MAX31855::MAX31855(SPI& spi, uint8_t device) : spi(spi), device(device) {}

uint16_t MAX31855::readTemp() {
    uint16_t temp;

    const uint8_t length = 4;
    uint8_t bytes[length];

    spi.startTransmission(device);
    spi.read(bytes, length);
    spi.endTransmission(device);

    temp = (((uint16_t) bytes[0]) << 8) | bytes[1];

    temp = temp >> 2;                             // lose last 2 bits as they aren't in the 14 bit temp
    temp = (temp >> 2) * 100 + (temp & 0x03) * 25;// convert last 2 digits to allow for decimal place
    return temp;
}

}// namespace TMU::DEV
