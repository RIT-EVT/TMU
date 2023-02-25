#include <TMU.hpp>

namespace TMU {
CO_OBJ_T* TMU::getObjectDictionary() {
    return objectDictionary;
}

uint16_t TMU::getObjectDictionarySize() const {
    return OBJECT_DICTIONARY_SIZE;
}

}// namespace TMU
