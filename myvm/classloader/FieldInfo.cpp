/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "FieldInfo.h"
#include "AttributeInfo.h"

#include <iostream>
#include <memory>
using namespace std;

using namespace myvm;

namespace myvm {

FieldInfo::FieldInfo(ClassFileInfo *clazz,
    uint16_t flags,
    uint16_t name,
    uint16_t desc,
    vector<AttributeInfo*> *attrs) {
    mOwnerClazz = clazz;
    accessFlags = flags;
    nameIndex = name;
    descriptorIndex = desc;
    attributeCount = attrs->size();
    mAttributes = *attrs;
}

FieldInfo::~FieldInfo() {

}

FieldInfo* FieldInfo::loadFromFile(ClassFileInfo *classFileInfo, FileReader *fileReader) {
    uint16_t accessFlags = 0;
    int status = fileReader->readUint16(accessFlags);
    if (status != 0) {
        return nullptr;
    }

    uint16_t nameIndex = 0;
    status = fileReader->readUint16(nameIndex);
    if (status != 0) {
        return nullptr;
    }
    classFileInfo->printConstantInfo(nameIndex);

    uint16_t descriptorIndex = 0;
    status = fileReader->readUint16(descriptorIndex);
    if (status != 0) {
        return nullptr;
    }

    uint16_t attributeCount = 0;
    status = fileReader->readUint16(attributeCount);
    if (status != 0) {
        return nullptr;
    }

    cout << "Have "<< attributeCount <<" attributes!" << endl;
    auto attributes = new vector<AttributeInfo *>();
    if (attributeCount > 0) {
        for (int i = 0; i < attributeCount; i++) {
            cout << "Load attribute #" << i << endl;
            AttributeInfo *attrInfo = AttributeFactory::loadFromFile(classFileInfo, fileReader);
            if (attrInfo == nullptr) {
                cout << "Load attribute #" << i << " failed!" << endl;
                return nullptr;
            }
            attributes->push_back(attrInfo);
            cout << "Load attribute #" << i << " complete!" << endl;
        }
    }
    return new FieldInfo(classFileInfo, accessFlags, nameIndex, descriptorIndex, attributes);
}

void FieldInfo::resolve() {
    ConstantUtf8 *name = mOwnerClazz->getUtf8Constant(nameIndex);
    if (name == nullptr) {
        // Error;
        return;
    }
    ConstantUtf8 *desc = mOwnerClazz->getUtf8Constant(descriptorIndex);
    if (desc == nullptr) {
        return;
    }

    mTypeInfo = TypeInfo::parseFrom(desc);
}

void FieldInfo::updateOffset(uint32_t offset) {
    mOffsetInClass = offset;
}

uint32_t FieldInfo::offsetInClass() {
    return mOffsetInClass;
}

}