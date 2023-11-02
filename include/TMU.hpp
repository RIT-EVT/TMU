#pragma once

#include <Canopen/co_core.h>
#include <EVT/io/CANopen.hpp>
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
    static constexpr uint16_t OBJECT_DICTIONARY_SIZE = 41;

    /**
     * CAN Open object dictionary
     */
    CO_OBJ_T objectDictionary[OBJECT_DICTIONARY_SIZE + 1] = {
        // Sync ID, defaults to 0x80
        {
            CO_KEY(0x1005, 0, CO_UNSIGNED32 | CO_OBJ_D__R_),
            nullptr,
            (uintptr_t) 0x80,
        },
        /**
         * Information about the hardware , hard coded sample values for now
         * 1: Vendor ID
         * 2: Product Code
         * 3: Revision Number
         * 4: Serial Number
         */
        {
            .Key = CO_KEY(0x1018, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x10,
        },
        {
            .Key = CO_KEY(0x1018, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x11,
        },
        {
            .Key = CO_KEY(0x1018, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x12,
        },
        {
            .Key = CO_KEY(0x1018, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x13,
        },

        /*
         * SDO CAN message IDS.
         * 1: Client -> Server ID, default is 0x600 + NODE_ID
         * 2: Server -> Client ID, default is 0x580 + NODE_ID
         */
        {
            .Key = CO_KEY(0x1200, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x600 + NODE_ID,
        },
        {
            .Key = CO_KEY(0x1200, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0x580 + NODE_ID,
        },

        /*
         * TPDO 0 Settings
         * 0. The number of sub indices.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1800, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5,
        },
        {
            //180h+Node-ID
            .Key = CO_KEY(0x1800, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(0) + NODE_ID,
        },
        {
            //timer triggered
            .Key = CO_KEY(0x1800, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            //no inhibit time
            .Key = CO_KEY(0x1800, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            //send every 2 seconds
            .Key = CO_KEY(0x1800, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 500,
        },

        /*
         * TPDO 0 Mapping
         * 0. The number of mapping objects in the first TPDO
         * 1. Link to the first temperature - thermTemps[0]
         * 2. Link to the second temperature - thermTemps[1]
         * 3. Link to the third temperature - thermTemps[2]
         * 4. Link to the first temperature - thermTemps[3]
         */
        {
            .Key = CO_KEY(0x1A00, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4,
        },
        {
            .Key = CO_KEY(0x1A00, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 0, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 1, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 2, 16),
        },
        {
            .Key = CO_KEY(0x1A00, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2100, 3, 16),
        },

        /*
         * TPDO 1 Settings
         * 0. The number of sub indices.
         * 1. The COBID for the transmitting node
         * 2. The transmission trigger 0xFE is asynchronous
         * 3. The inhibit time
         * 4. The event timer
         */
        {
            .Key = CO_KEY(0x1801, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 5,
        },
        {
            //180h+Node-ID
            .Key = CO_KEY(0x1801, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) CO_COBID_TPDO_DEFAULT(1) + NODE_ID,
        },
        {
            //timer triggered
            .Key = CO_KEY(0x1801, 2, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0xFE,
        },
        {
            //no inhibit time
            .Key = CO_KEY(0x1801, 3, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 0,
        },
        {
            //send every 2 seconds
            .Key = CO_KEY(0x1801, 5, CO_UNSIGNED16 | CO_OBJ_D__R_),
            .Type = CO_TEVENT,
            .Data = (uintptr_t) 500,
        },

        /*
         * TPDO 1 Mapping
         * 0. The number of mapping objects in the second TPDO
         * 1. Link to the first error - err_arr[0]
         * 2. Link to the second error - err_arr[1]
         * 3. Link to the third error - err_arr[2]
         * 4. Link to the fourth error - err_arr[3]
         */
        {
            .Key = CO_KEY(0x1A01, 0, CO_UNSIGNED8 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = (uintptr_t) 4,
        },
        {
            .Key = CO_KEY(0x1A01, 1, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 0, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 2, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 1, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 3, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 2, 8),
        },
        {
            .Key = CO_KEY(0x1A01, 4, CO_UNSIGNED32 | CO_OBJ_D__R_),
            .Type = nullptr,
            .Data = CO_LINK(0x2101, 3, 8),
        },

        /*
         * Data Links
         */
        {
            .Key = CO_KEY(0x2100, 0, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[0],
        },
        {
            .Key = CO_KEY(0x2100, 1, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[1],
        },
        {
            .Key = CO_KEY(0x2100, 2, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[2],
        },
        {
            .Key = CO_KEY(0x2100, 3, CO_UNSIGNED16 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &thermTemps[3],
        },

        /*
         * Error Data Links
         */
        {
            .Key = CO_KEY(0x2101, 0, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &err_arr[0],
        },
        {
            .Key = CO_KEY(0x2101, 1, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &err_arr[1],
        },
        {
            .Key = CO_KEY(0x2101, 2, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &err_arr[2],
        },
        {
            .Key = CO_KEY(0x2101, 3, CO_UNSIGNED8 | CO_OBJ___PRW),
            .Type = nullptr,
            .Data = (uintptr_t) &err_arr[3],
        },
        CO_OBJ_DIR_ENDMARK,
    };
};

}// namespace TMU
