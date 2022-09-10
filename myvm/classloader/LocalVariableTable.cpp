#include "LocalVariableTable.h"
#include "AttributeInfo.h"

namespace myvm {

LocalVariableTable::LocalVariableTable(Method* method, uint16_t size) {
    mLocalVariableArray = new uint8_t[size];

    ClassFileInfo *clazz = method->getClass();
    
    CodeAttr* mCodeAttr = nullptr;
    //mCodeAttr->
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