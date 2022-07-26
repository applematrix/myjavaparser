#include "FieldInfo.h"
#include "AttributeInfo.h"
using namespace myvm;

namespace myvm {

FieldInfo::FieldInfo(uint16_t flags,
    uint16_t name,
    uint16_t desc,
    vector<AttributeInfo*> *attrs) {
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

    auto attributes = new vector<AttributeInfo *>();
    if (attributeCount > 0) {
        for (int i = 0; i < attributeCount; i++) {
            AttributeInfo *attrInfo = AttributeFactory::loadFromFile(classFileInfo, fileReader);
            if (attrInfo == nullptr) {
                return nullptr;
            }
            attributes->push_back(attrInfo);
        }
    }
    return new FieldInfo(accessFlags, nameIndex, descriptorIndex, attributes);
}

}