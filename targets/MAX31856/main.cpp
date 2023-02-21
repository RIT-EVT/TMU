#include "EVT/utils/time.hpp"
#include "dev/MAX31856.h"
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>

namespace IO = EVT::core::IO;

constexpr uint32_t SPI_SPEED = SPI_SPEED_1MHZ;
constexpr uint8_t deviceCount = 1;
IO::GPIO* devices[deviceCount];

// need to verify spi settings:
// https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31855.pdf

int main() {
    IO::init();
    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup SPI
    devices[0] = &IO::getGPIO<IO::Pin::SPI_CS>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI, IO::Pin::SPI_MISO>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE0, SPI_MSB_FIRST);

    MAX31856::MAX31856 MAX(spi);

    //int32_t temp = MAX.readTemp();
    const int length = 4;
    uint8_t bytes[length];
    uint16_t temp = 0;
    uart.printf("read start:\r\n");

    while (true) {
        spi.startTransmission(0);
        spi.read(bytes, length);
        spi.endTransmission(0);
        temp = (((uint16_t)bytes[0]) << 8) | bytes[1];
        temp = temp >> 2; // lose last 2 bits as they aren't in the 14 bit temp
        temp = temp >> 2; // move over 2 more for decimal
        uart.printf("temp: %d\r\n", temp);
        break;
        EVT::core::time::wait(3000);
    }
}