#include "AttributeInfo.h"

namespace myvm {

CodeAttr::CodeAttr(ClassFileInfo *classFileInfo, FileReader* fileReader) : AttributeInfo(fileReader) {
    initialize(classFileInfo, fileReader);
}

CodeAttr::CodeAttr(uint16_t name, uint32_t len, ClassFileInfo *classFileInfo, FileReader *fileReader)
    : AttributeInfo(name, len) {
    initialize(classFileInfo, fileReader);
}

void CodeAttr::initialize(ClassFileInfo *classFileInfo, FileReader* fileReader) {
    fileReader->readUint16(maxStack);
    fileReader->readUint16(maxStack);
    fileReader->readUint32(codeLength);

    code = new uint8_t[codeLength];
    fileReader->read(code, codeLength);

    loadExceptionTable(fileReader);
    fileReader->readUint16(attrCounts);

    attributes = new AttributeInfo*[attrCounts];
    for (int i = 0; i < attrCounts; i++) {
        attributes[i] = AttributeFactory::loadFromFile(classFileInfo, fileReader);
    }
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

void RecordAttr::initialize(ClassFileInfo *classFileInfo, FileReader* fileReader){
    fileReader->readUint16(componentCount);
    for (int i = 0; i < componentCount; i++) {
        fileReader->readUint16(components[i].nameIndex);
        fileReader->readUint16(components[i].descriptorIndex);
        fileReader->readUint16(components[i].attributeCount);

        if (components[i].attributeCount == 0) {
            continue;
        }
        components[i].attributeInfo = new AttributeInfo*[components[i].attributeCount];
        for (int j = 0; j < components[i].attributeCount; j++) {
            components[i].attributeInfo[j] = AttributeFactory::loadFromFile(classFileInfo, fileReader);
        }
    }
}

} // namespace