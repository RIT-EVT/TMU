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
class TMU : public CANDevice {
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
     * Updates the temperature values in an array and updates the error array from the TMU object.
     */
    void process();

    /**
     * Get the device's node ID
     *
     * @return The node ID of the can device.
     */
    uint8_t getNodeID() override;

    /**
     * Get the number of elements in the object dictionary.
     *
     * @return The number of elements in the object dictionary
     */
    uint8_t getNumElements() override;

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
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 43;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        MANDATORY_IDENTIFICATION_ENTRIES_1000_1014,
        HEARTBEAT_PRODUCER_1017(2000),
        IDENTITY_OBJECT_1018,
        SDO_CONFIGURATION_1200,

        // TPDO 0 CONFIGURATION
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x00, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),
        // TPDO 1 CONFIGURATION
        TRANSMIT_PDO_SETTINGS_OBJECT_18XX(0x01, TRANSMIT_PDO_TRIGGER_TIMER, TRANSMIT_PDO_INHIBIT_TIME_DISABLE, 2000),

        // TPDO 0 MAPPING
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(0, 4),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 1, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 2, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 3, PDO_MAPPING_UNSIGNED16),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x00, 4, PDO_MAPPING_UNSIGNED16),

        // TPDO 1 MAPPING
        TRANSMIT_PDO_MAPPING_START_KEY_1AXX(1, 4),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 1, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 2, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 3, PDO_MAPPING_UNSIGNED8),
        TRANSMIT_PDO_MAPPING_ENTRY_1AXX(0x01, 4, PDO_MAPPING_UNSIGNED8),

        // TPDO 0 DATA LINKS
        DATA_LINK_START_KEY_21XX(0, 4),
        DATA_LINK_21XX(0x00, 1, CO_TUNSIGNED16, (uintptr_t) &thermTemps[0]),
        DATA_LINK_21XX(0x00, 2, CO_TUNSIGNED16, (uintptr_t) &thermTemps[1]),
        DATA_LINK_21XX(0x00, 3, CO_TUNSIGNED16, (uintptr_t) &thermTemps[2]),
        DATA_LINK_21XX(0x00, 4, CO_TUNSIGNED16, (uintptr_t) &thermTemps[3]),

        // TPDO 1 DATA LINKS
        DATA_LINK_START_KEY_21XX(1, 4),
        DATA_LINK_21XX(0x01, 1, CO_TUNSIGNED8, (uintptr_t) &err_arr[0]),
        DATA_LINK_21XX(0x01, 2, CO_TUNSIGNED8, (uintptr_t) &err_arr[1]),
        DATA_LINK_21XX(0x01, 3, CO_TUNSIGNED8, (uintptr_t) &err_arr[2]),
        DATA_LINK_21XX(0x01, 4, CO_TUNSIGNED8, (uintptr_t) &err_arr[3]),

        CO_OBJ_DICT_ENDMARK,
    };
};

}// namespace TMU
