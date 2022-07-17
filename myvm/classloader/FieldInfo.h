#ifndef _FIELD_INFO_H_
#define _FIELD_INFO_H_

#include "common/types.h"
#include "AttributeInfo.h"

namespace myvm {

class FieldInfo {
public:
    FieldInfo();
    ~FieldInfo();
private:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    AttributeInfo *mAttributes;
};

}
#endif