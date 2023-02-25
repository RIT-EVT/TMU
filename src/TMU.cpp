#include <TMU.hpp>

namespace TMU {

TMU::TMU(SPI& spi) : spi(spi) {}

CO_OBJ_T* TMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t TMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

void TMU::updateTemps() {
    for (int i=0; i<4; i++) {
        thermTemps[i] = MAXs[i].readTemp();
    }
}

}// namespace TMU
