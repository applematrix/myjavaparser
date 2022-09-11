#ifndef _LOCAL_VARIABLE_TABLE_H_
#define _LOCAL_VARIABLE_TABLE_H_

#include "Method.h"
#include "AttributeInfo.h"

#include <stdint.h>
#include <array>
using namespace std;

namespace myvm {

class LocalVariableAttr
{
public:
    LocalVariableAttr() {}
    ~LocalVariableAttr() {}
    uint16_t startPc;
    uint16_t length;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t index;
};

class LocalVariableTableAttr: public AttributeInfo {
public:
    uint16_t tableLen;
    vector<shared_ptr<LocalVariableAttr>> localVariables;

    LocalVariableTableAttr(FileReader* fileReader)
        : AttributeInfo(fileReader, ATTR_LOCAL_VARIABLE_TABLE) {
        initialize(fileReader);
    }

    LocalVariableTableAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_LOCAL_VARIABLE_TABLE) {
        initialize(fileReader);
    }

private:
    void initialize(FileReader* fileReader);
};

class LocalVariableTable {
public:
    LocalVariableTable(Method* method, uint16_t size);
    ~LocalVariableTable();

    void storeUint32(uint16_t index, uint32_t value);
    void storeUint64(uint16_t index, uint64_t value);
    uint32_t variableAt(uint16_t index);
    uint64_t longVariableAt(uint16_t index);
private:
    uint32_t *mLocalVariables;
    shared_ptr<LocalVariableTableAttr> mAttrbutes;
};

}
#endif