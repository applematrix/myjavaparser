/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _FIELD_INFO_H_
#define _FIELD_INFO_H_

#include <vector>
#include <memory>

#include "FileReader.h"
#include "TypeInfo.h"

using namespace myvm;
using namespace std;

namespace myvm {
struct AttributeInfo;
class ClassInfo;

struct FieldInfo {
public:
    FieldInfo(ClassInfo *clazz, 
        uint16_t flags, uint16_t name, uint16_t desc, vector<AttributeInfo*> *attrs);
    ~FieldInfo();
    void resolve();
    void updateOffset(uint32_t offset);
    uint32_t offsetInClass();
    shared_ptr<TypeInfo> getType() {
        return mTypeInfo;
    }

    static FieldInfo* loadFromFile(ClassInfo *classFileInfo, shared_ptr<FileReader> fileReader);
public:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;

private:
    vector<AttributeInfo*> mAttributes;
    ClassInfo* mOwnerClazz;
    shared_ptr<TypeInfo> mTypeInfo;

    // runtime properties
    uint32_t mOffsetInClass;
};

typedef FieldInfo MethodInfo;
}

#endif