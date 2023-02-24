#include "EVT/utils/time.hpp"
#include <EVT/io/UART.hpp>
#include <EVT/io/manager.hpp>
#include <EVT/io/pin.hpp>
#include <dev/MAX31855.h>

namespace IO = EVT::core::IO;

constexpr uint32_t SPI_SPEED = SPI_SPEED_1MHZ;
constexpr uint8_t deviceCount = 1;
IO::GPIO* devices[deviceCount];

int main() {
    IO::init();
    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup SPI
    devices[0] = &IO::getGPIO<IO::Pin::SPI_CS>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);
    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI, IO::Pin::SPI_MISO>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE0, SPI_MSB_FIRST);

    DEV::MAX31855 MAX(spi);

    uart.printf("read start:\r\n");
    while (true) {
        uint16_t temp = MAX.readTemp();
        ;
        uart.printf("%d.%02d\r\n", temp / 100, temp % 100);
        EVT::core::time::wait(200);
    }
}
