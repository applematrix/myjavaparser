#include "CodeAttr.h"
#include "Instructions.h"
#include <iostream>

#undef LOG_TAG
#define LOG_TAG "CodeAttr"
#include "../common/Logger.h"

using namespace myvm;
using namespace std;

namespace myvm{

CodeAttr::CodeAttr(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader)
    : AttributeInfo(fileReader, ATTR_CODE) {
    initialize(classFileInfo, fileReader);
}

CodeAttr::CodeAttr(uint16_t name, uint32_t len, ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader)
    : AttributeInfo(name, len, ATTR_CODE) {
    initialize(classFileInfo, fileReader);
}

void CodeAttr::initialize(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader) {
    fileReader->readUint16(maxStack);
    fileReader->readUint16(maxLocals);
    fileReader->readUint32(codeLength);

    code = new uint8_t[codeLength];
    fileReader->read(code, codeLength);

    loadExceptionTable(fileReader);
    fileReader->readUint16(attrCounts);

    attributes.reserve(attrCounts);
    LOGI("Code attributes:");
    for (int i = 0; i < attrCounts; i++) {
        //cout << "   ";
        AttributeInfo *attrInfo  = AttributeFactory::loadFromFile(classFileInfo, fileReader);
        attributes.push_back(shared_ptr<AttributeInfo>(attrInfo));
    }
    LOGI("Code attributes end!");

    LOGD("Code instructions section begin:");
    dumpCode();
    LOGD("Code instructions section end;");
}

void CodeAttr::loadExceptionTable(shared_ptr<FileReader> fileReader) {
    fileReader->readUint16(exceptionTableLength);
    exceptionTable = new ExceptionTable[exceptionTableLength];

    for (int i = 0; i < exceptionTableLength; i++) {
        fileReader->readUint16(exceptionTable[i].startPc);
        fileReader->readUint16(exceptionTable[i].endPc);
        fileReader->readUint16(exceptionTable[i].handlerPc);
        fileReader->readUint16(exceptionTable[i].catchType);
    }        
}

void CodeAttr::dumpCode() {
    uint8_t *opcode = code;
    for (uint32_t i = 0; i < codeLength; i++) {
        LOGD("instruction:%s", Instruction::getOpCodeDesc(opcode[i]));
    }
}

shared_ptr<AttributeInfo> CodeAttr::getAttributeAt(uint16_t index) {
    if (index >= attributes.size()) {
        return nullptr;
    }
    return attributes.at(index);
}

shared_ptr<AttributeInfo> CodeAttr::getAttributeByType(uint8_t type) {
    for (auto attr : attributes) {
        if (attr->attrType == type) {
            return attr;
        }
    }
    return nullptr;
}


}

