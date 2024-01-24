/**
* This program reads temperatures from MAX31855 sensors and prints them,
* along with error statuses, in a CSV-friendly format over UART.
*
* CSV format: temp1:, temp2:, temp3:, temp4:, error1:, error2:, error3:, error4:
*/

#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/time.hpp>

#include <dev/MAX31855.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;
namespace DEV = EVT::core::DEV;

int main() {
    // Initialize system
    EVT::core::platform::init();

    // Setup UART
    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);

    // Initialize SPI
    IO::GPIO* devices[4];

    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[1] = &IO::getGPIO<IO::Pin::PB_5>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[2] = &IO::getGPIO<IO::Pin::PB_6>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[3] = &IO::getGPIO<IO::Pin::PB_7>(EVT::core::IO::GPIO::Direction::OUTPUT);

    devices[0]->writePin(IO::GPIO::State::HIGH);
    devices[1]->writePin(IO::GPIO::State::HIGH);
    devices[2]->writePin(IO::GPIO::State::HIGH);
    devices[3]->writePin(IO::GPIO::State::HIGH);

    IO::SPI& spi = IO::getSPI<IO::Pin::PA_5, IO::Pin::PA_7, IO::Pin::PA_6>(devices, 4);

    spi.configureSPI(SPI_SPEED_1MHZ, SPI_MODE0, SPI_MSB_FIRST);

    TMU::DEV::MAX31855 MAXES[] = {
        TMU::DEV::MAX31855(spi, 0),
        TMU::DEV::MAX31855(spi, 1),
        TMU::DEV::MAX31855(spi, 2),
        TMU::DEV::MAX31855(spi, 3),
    };


    while (true) {
        //print temps and error array for csv friendly format
        uint16_t temps[4];
        TMU::DEV::MAX31855::MaxStatus errors[4];
        for (int i = 0; i < 4; i++) {
            errors[i] = MAXES[i].readTemp(temps[i]);
        }
        uart.printf("%d, %d, %d, %d, %d, %d, %d, %d\r\n", temps[0], temps[1], temps[2], temps[3], errors[0], errors[1], errors[2], errors[3]);

        time::wait(200);
    }
}
