#include "CodeAttr.h"
#include "Instructions.h"
#include <iostream>

using namespace myvm;
using namespace std;

namespace myvm{

CodeAttr::CodeAttr(ClassFileInfo *classFileInfo, FileReader* fileReader)
    : AttributeInfo(fileReader, ATTR_CODE) {
    initialize(classFileInfo, fileReader);
}

CodeAttr::CodeAttr(uint16_t name, uint32_t len, ClassFileInfo *classFileInfo, FileReader *fileReader)
    : AttributeInfo(name, len, ATTR_CODE) {
    initialize(classFileInfo, fileReader);
}

void CodeAttr::initialize(ClassFileInfo *classFileInfo, FileReader* fileReader) {
    fileReader->readUint16(maxStack);
    fileReader->readUint16(maxLocals);
    fileReader->readUint32(codeLength);

    code = new uint8_t[codeLength];
    fileReader->read(code, codeLength);

    loadExceptionTable(fileReader);
    fileReader->readUint16(attrCounts);

    attributes.reserve(attrCounts);
    cout << "Code attributes:" << endl;
    for (int i = 0; i < attrCounts; i++) {
        cout << "   ";
        AttributeInfo *attrInfo  = AttributeFactory::loadFromFile(classFileInfo, fileReader);
        attributes.push_back(shared_ptr<AttributeInfo>(attrInfo));
    }
    cout << "Code attributes end!" << endl;

    cout << "Code instructions:" << endl;
    dumpCode();
    cout << endl;
}

void CodeAttr::loadExceptionTable(FileReader* fileReader) {
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
        cout << Instruction::getOpCodeDesc(opcode[i]) << endl;
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

