#include <EVT/io/SPI.hpp>
#include <dev/MAX31855.hpp>
#include <utility>

using namespace EVT::core::IO;

namespace TMU::DEV {

MAX31855::MAX31855(SPI& spi, uint8_t device) : spi(spi), device(device) {}

MAX31855::MaxStatus MAX31855::readTemp(uint16_t& temp) {
    uint8_t lastDataBit = 0;
    uint16_t returned_temp = 0;

    const uint8_t length = 4;
    uint8_t bytes[length] = {'\0'};

    spi.startTransmission(device);
    spi.read(bytes, length);
    spi.endTransmission(device);

    returned_temp = (((uint16_t) bytes[0]) << 8) | bytes[1];

    lastDataBit = ((bytes[1]) << 7);// Shift it 7 to get rid of internal temp data and reserve bit
    lastDataBit = lastDataBit >> 7; // Shift it back to the least significant position

    /*
     * 0x01 is the flag for OC Fault (Open Circuit)
     * 0x02 is the flag for SCG Fault (Short to GND)
     * 0x04 is the flag for SCV Fault (Short to VCC)

     Check the error checking bit, if bit equals 1, then there is an error
     */
    if (lastDataBit & 0x01) {
        uint8_t lastByte = 0;
        lastByte = bytes[3];
        lastByte = lastByte << 5;//Shift it 5 to get rid of internal temp data and reserve bit
        lastByte = lastByte >> 5;//Shift it back to the least significant position

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
        return MaxStatus::MAX31855_ERROR;
    }

    returned_temp = returned_temp >> 2;// Make temp equal to the 14-byte read temp value

    returned_temp = (returned_temp >> 2) * 100 + (returned_temp & 0x03) * 25;// Convert the last 2 digits to allow for a decimal place

    temp = returned_temp;

    return MaxStatus::NO_ERROR;
}

}// namespace TMU::DEV
