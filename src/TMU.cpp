#include <TMU.hpp>

namespace TMU {
CO_OBJ_T* TMU::getObjectDictionary() {
    return &objectDictionary[0];
}

uint16_t TMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

}// namespace TMU
