#include "LocalVariableTable.h"

namespace myvm {

LocalVariableTable::LocalVariableTable(uint16_t size) {
    mLocalVariableArray = new uint8_t[size];
}

}