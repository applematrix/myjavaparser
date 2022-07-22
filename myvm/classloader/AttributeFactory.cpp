#include "AttributeFactory.h"

namespace myvm {

struct {
    const char* attribute;
    uint8_t attributeType;
} AttributeMap;


static uint8_t getAttributeType(uint8_t* bytes, uint16_t len) {

}

AttributeInfo* AttributeFactory::loadFromFile(ClassFileInfo* classInfo, FileReader* fileReader) {
    uint16_t nameIndex = 0;
    int status = fileReader->readUint16(nameIndex);
    if (status != 0) {
        return nullptr;
    }

    uint16_t attrLength = 0;
    status = fileReader->readUint16(attrLength);
    if (status != 0) {
        return nullptr;
    }
    ConstantInfo* constantInfo = classInfo->getConstantAt(nameIndex);
    if (constantInfo == nullptr) {
        return nullptr;
    }

    uint8_t tag = constantInfo->tag;
    switch (tag)
    {
    case ConstantTag::Utf8:
        ConstantUtf8 *utf8Info = (ConstantUtf8 *)constantInfo;
        uint8_t type = getAttributeType(utf8Info->bytes, utf8Info->length);

        switch (type) {
            case AttributeType::ConstantValue:
                break;
            case AttributeType::Code:
                break;
        }
        break;
    
    default:
        break;
    }


    return nullptr;
}

}