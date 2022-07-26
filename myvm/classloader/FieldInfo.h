#ifndef _FIELD_INFO_H_
#define _FIELD_INFO_H_

#include <vector>

#include "common/types.h"

#include "FileReader.h"

using namespace myvm;

namespace myvm {
struct AttributeInfo;

struct FieldInfo {
public:
    FieldInfo();
    ~FieldInfo();
    static FieldInfo* loadFromFile(FileReader *fileReader);
private:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;

    vector<AttributeInfo*> mAttributes;
};

typedef FieldInfo MethodInfo;

}
#endif