#include <EVT/dev/MCUTimer.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <EVT/io/CANopen.hpp>
#include <EVT/io/UART.hpp>
#include <EVT/io/pin.hpp>
#include <EVT/manager.hpp>
#include <EVT/utils/log.hpp>
#include <EVT/utils/time.hpp>
#include <TMU.hpp>
#include <dev/MAX31855.hpp>

namespace IO = EVT::core::IO;
namespace log = EVT::core::log;
namespace time = EVT::core::time;
namespace DEV = EVT::core::DEV;

///////////////////////////////////////////////////////////////////////////////
// EVT-core CAN callback and CAN setup. This will include logic to set
// aside CANopen messages into a specific queue
///////////////////////////////////////////////////////////////////////////////

/**
 * Interrupt handler to get CAN messages. A function pointer to this function
 * will be passed to the EVT-core CAN interface which will in turn call this
 * function each time a new CAN message comes in.
 *
 * NOTE: For this sample, every non-extended (so 11 bit CAN IDs) will be
 * assumed to be intended to be passed as a CANopen message.
 *
 * @param message[in] The passed in CAN message that was read.
 */

// create a can interrupt handler
void canInterrupt(IO::CANMessage& message, void* priv) {
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>* queue =
        (EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage>*) priv;

    // Log raw received data
    log::LOGGER.log(log::Logger::LogLevel::DEBUG, "Got RAW message from %X of length %d with data: ", message.getId(), message.getDataLength());

    uint8_t* data = message.getPayload();
    for (int i = 0; i < message.getDataLength(); i++) {
        log::LOGGER.log(log::Logger::LogLevel::DEBUG, "%X ", *data);
        data++;
    }

    if (queue != nullptr)
        queue->append(message);
}

int main() {
    // Initialize system
    EVT::core::platform::init();

    IO::UART& uart = IO::getUART<IO::Pin::UART_TX, IO::Pin::UART_RX>(9600);
    log::LOGGER.setUART(&uart);

    // Initialize SPI
    IO::GPIO* devices[NUM_THERMOCOUPLES];

    devices[0] = &IO::getGPIO<IO::Pin::PB_4>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[1] = &IO::getGPIO<IO::Pin::PB_5>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[2] = &IO::getGPIO<IO::Pin::PB_6>(EVT::core::IO::GPIO::Direction::OUTPUT);
    devices[3] = &IO::getGPIO<IO::Pin::PB_7>(EVT::core::IO::GPIO::Direction::OUTPUT);

    devices[0]->writePin(IO::GPIO::State::HIGH);
    devices[1]->writePin(IO::GPIO::State::HIGH);
    devices[2]->writePin(IO::GPIO::State::HIGH);
    devices[3]->writePin(IO::GPIO::State::HIGH);

    IO::SPI& spi = IO::getSPI<IO::Pin::PA_5, IO::Pin::PA_7, IO::Pin::PA_6>(devices, NUM_THERMOCOUPLES);

    spi.configureSPI(SPI_SPEED_1MHZ, SPI_MODE0, SPI_MSB_FIRST);

    TMU::DEV::MAX31855 thermocouples[NUM_THERMOCOUPLES] = {
        TMU::DEV::MAX31855(spi, 0),
        TMU::DEV::MAX31855(spi, 1),
        TMU::DEV::MAX31855(spi, 2),
        TMU::DEV::MAX31855(spi, 3),
    };

    TMU::TMU tmu = TMU::TMU(thermocouples);

    DEV::Timer& timer = DEV::getTimer<DEV::MCUTimer::Timer2>(100);

    ///////////////////////////////////////////////////////////////////////////
    // Setup CAN configuration, this handles making drivers, applying settings.
    // And generally creating the CANopen stack node which is the interface
    // between the application (the code we write) and the physical CAN network
    ///////////////////////////////////////////////////////////////////////////

    // Will store CANopen messages that will be populated by the EVT-core CAN
    // interrupt
    EVT::core::types::FixedQueue<CANOPEN_QUEUE_SIZE, IO::CANMessage> canOpenQueue;

    // Initialize CAN, add an IRQ which will add messages to the queue above
    IO::CAN& can = IO::getCAN<IO::Pin::PA_12, IO::Pin::PA_11>();
    can.addIRQHandler(canInterrupt, reinterpret_cast<void*>(&canOpenQueue));

    // Attempt to join the CAN network
    IO::CAN::CANStatus result = can.connect();

    // Check to see if the device is connected to the CAN network
    if (result != IO::CAN::CANStatus::OK) {
        log::LOGGER.log(log::Logger::LogLevel::ERROR, "Failed to connect to CAN network\r\n");
        return 1;
    } else {
        log::LOGGER.log(log::Logger::LogLevel::INFO, "Connected to CAN network\r\n");
    }

    // Reserved memory for CANopen stack usage
    uint8_t sdoBuffer[CO_SSDO_N * CO_SDO_BUF_BYTE];
    CO_TMR_MEM appTmrMem[16];

    // Reserve driver variables
    CO_IF_DRV canStackDriver;

    CO_IF_CAN_DRV canDriver;
    CO_IF_TIMER_DRV timerDriver;
    CO_IF_NVM_DRV nvmDriver;

    CO_NODE canNode;

    // Initialize all the CANOpen drivers.
    IO::initializeCANopenDriver(&canOpenQueue, &can, &timer, &canStackDriver, &nvmDriver, &timerDriver, &canDriver);

    // Initialize the CANOpen node we are using.
    IO::initializeCANopenNode(&canNode, &tmu, &canStackDriver, sdoBuffer, appTmrMem);

    // Set the node to operational mode
    CONmtSetMode(&canNode.Nmt, CO_OPERATIONAL);

    time::wait(500);

    ///////////////////////////////////////////////////////////////////////////
    // Main loop
    ///////////////////////////////////////////////////////////////////////////

    while (1) {
        tmu.process();
        IO::processCANopenNode(&canNode);
        time::wait(1000);
    }
}
