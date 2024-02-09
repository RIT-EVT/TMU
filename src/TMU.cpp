#include <TMU.hpp>

namespace TMU {

TMU::TMU(DEV::MAX31855 thermocouples[NUM_THERMOCOUPLES]) : thermocouples{
    thermocouples[0], thermocouples[1], thermocouples[2], thermocouples[3]} {}

CO_OBJ_T* TMU::getObjectDictionary() {
    return objectDictionary;
}

uint8_t TMU::getNumElements() {
    return OBJECT_DICTIONARY_SIZE;
}

uint8_t TMU::getNodeID() {
    return NODE_ID;
}

void TMU::process() {
    for (int i = 0; i < NUM_THERMOCOUPLES; i++) {
        err_arr[i] = thermocouples[i].readTemp(thermTemps[i]);
        //Set the temp to int max if there was an error reading from the MAX31855
        if (err_arr[i] != DEV::MAX31855::MaxStatus::OK) {
            thermTemps[i] = -1;
        }
    }
}

}// namespace TMU
