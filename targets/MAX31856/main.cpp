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
// https://www.analog.com/media/en/technical-documentation/data-sheets/MAX31856.pdf

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
    uart.printf("read start:\r\n");

    for (int i=0; i<=15; i++) {
        spi.readReg(0, i, bytes, length);
        uart.printf("reg[%d] values: %x, %x\r\n", i, bytes[0], bytes[1]);
    }
}