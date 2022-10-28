/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "AttributeInfo.h"
#include "Instructions.h"
#include <cstring>
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
    if (type < 0 || type > ATTR_PERMITTED_SUBCLASS) {
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

void RecordAttr::initialize(ClassInfo *classFileInfo, FileReader* fileReader){
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
