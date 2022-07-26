#include "FieldInfo.h"
#include "AttributeInfo.h"
using namespace myvm;

namespace myvm {

FieldInfo::FieldInfo() {

}

FieldInfo::~FieldInfo() {

}

FieldInfo* FieldInfo::loadFromFile(FileReader *fileReader) {
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

    if (attributeCount > 0) {
        /*mAttributes.reserve(attributeCount);
        for (int i = 0; i < attributeCount; i++) {
            // TODO:
            mAttributes.push_back(nullptr);
        }*/
    }
    return nullptr;
}

}