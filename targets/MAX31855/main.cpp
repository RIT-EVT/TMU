#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/time.hpp>
#include <dev/MAX31855.hpp>

namespace IO = EVT::core::IO;

constexpr uint32_t SPI_SPEED = SPI_SPEED_1MHZ;
constexpr uint8_t deviceCount = 4;
IO::GPIO* devices[deviceCount];

int main() {
    EVT::core::platform::init();
    // Setup IO
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);

    // Setup SPI
    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    devices[1] = &IO::getGPIO<IO::Pin::PB_5>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[1]->writePin(IO::GPIO::State::HIGH);

    devices[2] = &IO::getGPIO<IO::Pin::PB_6>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[2]->writePin(IO::GPIO::State::HIGH);

    devices[3] = &IO::getGPIO<IO::Pin::PB_7>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[3]->writePin(IO::GPIO::State::HIGH);

    IO::SPI& spi = IO::getSPI<IO::Pin::SPI_SCK, IO::Pin::SPI_MOSI, IO::Pin::SPI_MISO>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE0, SPI_MSB_FIRST);

    TMU::DEV::MAX31855 MAX(spi, 0);

    uart.printf("read start:\r\n");
    while (true) {
        uint16_t temp = MAX.readTemp();
        uart.printf("%d.%02d\r\n", temp / 100, temp % 100);
        EVT::core::time::wait(200);
    }
}
