#pragma once

#include <EVT/io/CANDevice.hpp>
#include <EVT/io/CANOpenMacros.hpp>
#include <co_core.h>
#include <EVT/io/SPI.hpp>
#include <dev/MAX31855.hpp>

#define NUM_THERMOCOUPLES 4

using namespace EVT::core::IO;

namespace TMU {

/**
 * Handles thermo-couple data from the MAX31855 ADC and sends it over CANopen
 */
class TMU {
public:
    /**
     * Constructor takes an array 4 thermocouples
     *
     * @param thermocouples the MAX31855 thermocouples
     */
    explicit TMU(DEV::MAX31855 thermocouples[NUM_THERMOCOUPLES]);

    static constexpr uint8_t NODE_ID = 7;

    /**
     * Gets the object dictionary
     *
     * @return an object dictionary
     */
    CO_OBJ_T* getObjectDictionary();

    /**
     * Gets the size of the Object Dictionary
     *
     * @return size of the Object Dictionary
     */
    uint16_t getObjectDictionarySize() const;

    /**
     * Updates the temperature values in an array and updates the error array from the TMU object.
     */
    void process();

private:
    /**
     * Stores the 4 16-bit temperature values.
     */
    uint16_t thermTemps[NUM_THERMOCOUPLES] = {};

    /**
     * Stores the 4 MAX31855 thermocouples
     */
    DEV::MAX31855 thermocouples[NUM_THERMOCOUPLES];

    /*
     * Stores the 4 MAX31855 error statuses
     */
    DEV::MAX31855::MaxStatus err_arr[NUM_THERMOCOUPLES];

    /**
     * Object Dictionary Size
     */
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 69;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        // TPDO 0 Settings
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x05),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1800, 0x01, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1800, 0x02, PDO_MAPPING_UNSIGNED32),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1800, 0x03, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1800, 0x04, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1800, 0x05, PDO_MAPPING_UNSIGNED16),

        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x01, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0x00, 0x05),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1801, 0x01, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1801, 0x02, PDO_MAPPING_UNSIGNED32),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1801, 0x03, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1801, 0x04, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x1801, 0x05, PDO_MAPPING_UNSIGNED16),

        DATA_LINK_START_KEY_21XX(0, 0x05),
        DATA_LINK_21XX(0x1A00, 0x01, CO_TUNSIGNED32, (uintptr_t) &thermTemps[0]),
        DATA_LINK_21XX(0x1A00, 0x02, CO_TUNSIGNED32, (uintptr_t) &thermTemps[1]),
        DATA_LINK_21XX(0x1A00, 0x03, CO_TUNSIGNED32, (uintptr_t) &thermTemps[2]),
        DATA_LINK_21XX(0x1A00, 0x04, CO_TUNSIGNED32, (uintptr_t) &thermTemps[3]),

        DATA_LINK_START_KEY_21XX(1, 0x05),
        DATA_LINK_21XX(0x1A01, 0x01, CO_TUNSIGNED32, (uintptr_t) &err_arr[0]),
        DATA_LINK_21XX(0x1A01, 0x02, CO_TUNSIGNED32, (uintptr_t) &err_arr[1]),
        DATA_LINK_21XX(0x1A01, 0x03, CO_TUNSIGNED32, (uintptr_t) &err_arr[2]),
        DATA_LINK_21XX(0x1A01, 0x04, CO_TUNSIGNED32, (uintptr_t) &err_arr[3]),

        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace TMU
