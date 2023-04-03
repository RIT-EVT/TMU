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
    IO::UART& uart = IO::getUART<IO::Pin::PA_2, IO::Pin::PA_3>(9600);

    // Setup SPI
    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[0]->writePin(IO::GPIO::State::HIGH);

    devices[1] = &IO::getGPIO<IO::Pin::PB_5>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[1]->writePin(IO::GPIO::State::HIGH);

    devices[2] = &IO::getGPIO<IO::Pin::PB_6>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[2]->writePin(IO::GPIO::State::HIGH);

    devices[3] = &IO::getGPIO<IO::Pin::PB_7>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[3]->writePin(IO::GPIO::State::HIGH);

    IO::SPI& spi = IO::getSPI<IO::Pin::PA_5, IO::Pin::SPI_MOSI, IO::Pin::PA_6>(devices, deviceCount);
    spi.configureSPI(SPI_SPEED, SPI_MODE0, SPI_MSB_FIRST);

    TMU::DEV::MAX31855 MAX1(spi, 0);
    TMU::DEV::MAX31855 MAX2(spi, 1);
    TMU::DEV::MAX31855 MAX3(spi, 2);
    TMU::DEV::MAX31855 MAX4(spi, 3);


    uart.printf("read start:\r\n");
    while (true) {
        uint16_t temp = MAX1.readTemp();
        uart.printf("MAX 1: %d.%02d\r\n", temp / 100, temp % 100);

        temp = MAX2.readTemp();
        uart.printf("MAX 2: %d.%02d\r\n", temp / 100, temp % 100);

        temp = MAX3.readTemp();
        uart.printf("MAX 3: %d.%02d\r\n", temp / 100, temp % 100);

        temp = MAX4.readTemp();
        uart.printf("MAX 4: %d.%02d\r\n", temp / 100, temp % 100);

        EVT::core::time::wait(200);
    }
}
