#include "AttributeInfo.h"

#include <iostream>
using namespace std;

namespace myvm {

const char* attrNames[] = {
    "ConstantValue",
    "Code",
    "StackMapTable",
    "Exceptions",
    "InnerClasses",
    "EnclosingMethod",
    "Synthetic",
    "Signature",
    "SourceFile",
    "SourceDebugExtension",
    "LineNumberTable",
    "LocalVariableTable",
    "LocalVariableTypeTable",
    "Deprecated",
    "RuntimeVisibleAnnotations",
    "RuntimeInvisibleAnnotations",
    "AnnotationDefault",
    "BootstrapMethods",
    "MethodParameters",
    "Module",
    "ModulePackage",
    "ModuleMainClass",
    "NestHost",
    "NestMember",
    "Record",
    "PermittedSubClass",
};

const char* nameOfAttrType(uint8_t type) {
    if (type < 0 || type > PermittedSubClass) {
        return "Unknown attribute";
    }
    return attrNames[type];
}

uint8_t getAttributeType(uint8_t *bytes, uint16_t len) {
    uint8_t mapSize = sizeof(attrNames)/sizeof(attrNames[0]);
    for (uint8_t i = 0; i < mapSize; i++) {
        if (!strncmp(attrNames[i], (char*)bytes, len)) {
            return i;
        }
    }
    return -1;
}

CodeAttr::CodeAttr(ClassFileInfo *classFileInfo, FileReader* fileReader)
    : AttributeInfo(fileReader) {
    initialize(classFileInfo, fileReader);
}

CodeAttr::CodeAttr(uint16_t name, uint32_t len, ClassFileInfo *classFileInfo, FileReader *fileReader)
    : AttributeInfo(name, len) {
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

    attributes = new AttributeInfo*[attrCounts];
    cout << "Code attributes:" << endl;
    for (int i = 0; i < attrCounts; i++) {
        cout << "   ";
        attributes[i] = AttributeFactory::loadFromFile(classFileInfo, fileReader);
    }
    cout << "Code attributes end!" << endl;
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