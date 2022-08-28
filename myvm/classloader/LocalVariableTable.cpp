#include "LocalVariableTable.h"

namespace myvm {

LocalVariableTable::LocalVariableTable(uint16_t size) {
    mLocalVariableArray = new uint8_t[size];
}

void LocalVariableTable::store(uint16_t index, uint32_t value) {
    // TODO:
    //mLocalVariableArray = new uint8_t[size];
}

uint32_t LocalVariableTable::variableAt(uint16_t index) {
    // TODO:
    //mLocalVariableArray = new uint8_t[size];
    return 0;
}

}