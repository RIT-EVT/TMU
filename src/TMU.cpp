#include <TMU.hpp>

namespace TMU {

TMU::TMU(DEV::MAX31855 thermocouples[NUM_THERMOCOUPLES]) : thermocouples{
    thermocouples[0], thermocouples[1], thermocouples[2], thermocouples[3]} {}

CO_OBJ_T* TMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t TMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

void TMU::process() {
    uint16_t thermTemps[NUM_THERMOCOUPLES] = {};

    for (int i = 0; i < 4; i++) {
        err_arr[i] = thermocouples[i].readTemp(thermTemps[i]);

        //Set the temp to int max if there was an error reading from the MAX31855
        if (err_arr[i] != DEV::MAX31855::MaxStatus::OK) {
            thermTemps[i] = -1;
        }
    }
}

}// namespace TMU
