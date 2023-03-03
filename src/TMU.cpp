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

void TMU::updateTemps() {
    for (int i = 0; i < 4; i++) {
        thermTemps[i] = thermocouples[i].readTemp();
    }
}

}// namespace TMU
