#ifndef _CODE_ATTR_H_
#define _CODE_ATTR_H_

#include "AttributeInfo.h"

using namespace myvm;

namespace myvm {

class CodeAttr: public AttributeInfo {
public:
    uint16_t maxStack;
    uint16_t maxLocals;
    uint32_t codeLength;
    uint8_t *code;
    uint16_t exceptionTableLength;
    struct ExceptionTable {
        uint16_t startPc;
        uint16_t endPc;
        uint16_t handlerPc;
        uint16_t catchType;
    } *exceptionTable;
    uint16_t attrCounts;
    vector<shared_ptr<AttributeInfo>> attributes;

    CodeAttr(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader);
    CodeAttr(uint16_t name, uint32_t len, ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader);
    shared_ptr<AttributeInfo> getAttributeAt(uint16_t index);
    shared_ptr<AttributeInfo> getAttributeByType(uint8_t type);
    void initialize(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader);
    void loadExceptionTable(shared_ptr<FileReader> fileReader);
    void dumpCode();
};

}



#endif