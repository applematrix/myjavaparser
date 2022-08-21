/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _FIELD_INFO_H_
#define _FIELD_INFO_H_

#include <vector>

#include "FileReader.h"

using namespace myvm;
using namespace std;

namespace myvm {
struct AttributeInfo;
class ClassFileInfo;

struct FieldInfo {
public:
    FieldInfo(uint16_t flags, uint16_t name, uint16_t desc, vector<AttributeInfo*> *attrs);
    ~FieldInfo();
    static FieldInfo* loadFromFile(ClassFileInfo *classFileInfo, FileReader *fileReader);
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