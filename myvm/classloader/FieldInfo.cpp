/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#undef LOG_TAG
#define LOG_TAG "FieldInfo"
#include "../common/Logger.h"

#include "FieldInfo.h"
#include "AttributeInfo.h"

#include <iostream>
#include <memory>
using namespace std;

using namespace myvm;

namespace myvm {

FieldInfo::FieldInfo(ClassInfo *clazz,
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

FieldInfo* FieldInfo::loadFromFile(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader) {
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

    LOGI("Have %d attributes!", attributeCount);
    auto attributes = new vector<AttributeInfo *>();
    if (attributeCount > 0) {
        for (int i = 0; i < attributeCount; i++) {
            LOGI("Load attribute #%d", i);
            AttributeInfo *attrInfo = AttributeFactory::loadFromFile(classFileInfo, fileReader);
            if (attrInfo == nullptr) {
                LOGW("Load attribute #%d failed!", i);
                return nullptr;
            }
            attributes->push_back(attrInfo);
            LOGI("Load attribute #%d complete!", i);
        }
    }
    return new FieldInfo(classFileInfo, accessFlags, nameIndex, descriptorIndex, attributes);
}

void FieldInfo::resolve() {
    auto name = mOwnerClazz->getConstant<ConstantUtf8>(nameIndex);
    if (name == nullptr) {
        // Error;
        return;
    }
    auto desc = mOwnerClazz->getConstant<ConstantUtf8>(descriptorIndex);
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

bool FieldInfo::match(shared_ptr<ConstantNameAndType>& nameAndType) const {
    return match(nameAndType->nameIndex, nameAndType->descriptorIndex);
}

bool FieldInfo::match(uint16_t nameIndex, uint16_t descIndex) const {
    return this->nameIndex == nameIndex && this->descriptorIndex == descIndex;
}

}