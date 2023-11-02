#include <EVT/io/SPI.hpp>
#include <dev/MAX31855.hpp>

using namespace EVT::core::IO;

namespace TMU::DEV {

MAX31855::MAX31855(SPI& spi, uint8_t device) : spi(spi), device(device) {}

MAX31855::MaxStatus MAX31855::readTemp(uint16_t& temp) {
    uint16_t returnedData = 0;

    const uint8_t length = 4;
    uint8_t bytes[length];

    spi.startTransmission(device);
    SPI::SPIStatus status = spi.read(bytes, length);
    if (status != SPI::SPIStatus::OK) {
        return MaxStatus::SPI_ERROR;
    }
    spi.endTransmission(device);

    uint8_t lastDataBit = 0;
    lastDataBit = bytes[1] & 0x01;// Get the last bit of the 2nd byte

    /*
     * 0x01 is the flag for OC Fault (Open Circuit)
     * 0x02 is the flag for SCG Fault (Short to GND)
     * 0x04 is the flag for SCV Fault (Short to VCC)

     Check the error checking bit, if bit equals 1, then there is an error
     */
    if (lastDataBit & 0x01) {
        uint8_t lastByte = 0;
        lastByte = bytes[3] & 0x01;// Get the last bit of the 4th byte

        // Check for OC Fault
        if (lastByte & 0x01) {
            return MaxStatus::OC_FAULT;
        }

        // Check for SCG Fault
        if (lastByte & 0x02) {
            return MaxStatus::SCG_FAULT;
        }

        // Check for SCV Fault
        if (lastByte & 0x04) {
            return MaxStatus::SCV_FAULT;
        }

        // If the byte does not match any flags, then there is an error reading from the MAX31855
        return MaxStatus::READ_ERROR;
    }
    returnedData = (((uint16_t) bytes[0]) << 8) | bytes[1];

    returnedData = returnedData >> 2;// Make temp equal to the 14-byte read temp value

    returnedData = (returnedData >> 2) * 100 + (returnedData & 0x03) * 25;// Convert the last 2 digits to allow for a decimal place

    temp = returnedData;

    return MaxStatus::OK;
}

}// namespace TMU::DEV
