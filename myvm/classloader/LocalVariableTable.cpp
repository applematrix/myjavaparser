#include "LocalVariableTable.h"
#include "AttributeInfo.h"

namespace myvm {

void LocalVariableTableAttr::initialize(shared_ptr<FileReader> fileReader){
    fileReader->readUint16(tableLen);
    localVariables.reserve(tableLen);
    for (int i = 0; i < tableLen; i++)
    {
        LocalVariableAttr *localVariable = new LocalVariableAttr();
        fileReader->read(localVariable->startPc);
        fileReader->read(localVariable->length);
        fileReader->read(localVariable->nameIndex);
        fileReader->read(localVariable->descriptorIndex);
        fileReader->read(localVariable->index);
        localVariables.push_back(shared_ptr<LocalVariableAttr>(localVariable));
    }
}

//////////////////////////////////////////
LocalVariableTable::LocalVariableTable(shared_ptr<Method> method, uint16_t size) {
    ClassInfo *clazz = method->getClass();
    mLocalVariables = new uint32_t[size];
}

LocalVariableTable::~LocalVariableTable() {
    delete[] mLocalVariables;
}

void LocalVariableTable::storeUint32(uint16_t index, uint32_t value) {
    mLocalVariables[index] = value;
}

void LocalVariableTable::storeUint64(uint16_t index, uint64_t value) {
    mLocalVariables[index] = value & 0xffffffff;
    mLocalVariables[index + 1] = ((value >> 32) & 0xffffffff);
}

uint32_t LocalVariableTable::variableAt(uint16_t index) {
    return mLocalVariables[index];
}

uint64_t LocalVariableTable::longVariableAt(uint16_t index) {
    uint64_t ret = mLocalVariables[index + 1];
    return ((ret << 32) | mLocalVariables[index]);
}

}