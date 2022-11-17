/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CONSTANT_INFO_H_
#define _CONSTANT_INFO_H_
#include <stdint.h>
#include <string>

#undef LOG_TAG
#define LOG_TAG "ConstantInfo"
#include "../common/Logger.h"

using namespace std;

namespace myvm {

class ClassInfo;

enum ConstantTag {
    CONSTANT_UTF8 = 1,
    CONSTANT_INTEGER = 3,
    CONSTANT_FLOAT = 4,
    CONSTANT_LONG = 5,
    CONSTANT_DOUBLE = 6,
    CONSTANT_CLASS = 7,
    CONSTANT_STRING = 8,
    CONSTANT_FIELDREF = 9,
    CONSTANT_METHODREF = 10,
    CONSTANT_INTERFACE_METHODREF = 11,
    CONSTANT_NAMEANDTYPE = 12,
    CONSTANT_METHODHANDLE = 15,
    CONSTANT_METHODTYPE = 16,
    CONSTANT_DYNAMIC = 17,
    CONSTANT_INVOKEDYNAMIC = 18,
    CONSTANT_CONSTANT_MODULE =19,
    CONSTANT_PACKAGE = 20,
};

class ConstantInfo {
public:
    uint8_t tag;

    ConstantInfo(uint8_t _tag): tag(_tag) {}
    const char* typeString();
    virtual void dump(const ClassInfo* classInfo) {}
    virtual void resolve() {}
};

class ConstantClass: public ConstantInfo {
public:
    uint16_t nameIndex;

    ConstantClass(uint8_t tag, uint16_t index) :
        ConstantInfo(tag), nameIndex(index) {}
    virtual void dump(const ClassInfo* classInfo);
};

class ConstantRef: public ConstantInfo {
public:
    uint16_t classIndex;
    uint16_t nameAndTypeIndex;
    ConstantRef(uint8_t tag, uint16_t index, uint16_t nameAndType) : 
        ConstantInfo(tag), classIndex(index), nameAndTypeIndex(nameAndType) {}
};

class ConstantString: public ConstantInfo {
public:
    uint16_t stringIndex;
    ConstantString(uint16_t index) : 
        ConstantInfo(CONSTANT_STRING), stringIndex(index) {}
};

class ConstantInteger: public ConstantInfo {
public:
    uint32_t bytes;
    ConstantInteger(uint32_t _bytes) : 
        ConstantInfo(CONSTANT_INTEGER), bytes(_bytes) {}
    
    ConstantInteger(uint8_t tag, uint32_t _bytes) : 
        ConstantInfo(tag), bytes(_bytes) {}
};
typedef ConstantInteger ConstantFloat;

class ConstantLong: public ConstantInfo {
public:
    uint32_t highBytes;
    uint32_t lowBytes;

    ConstantLong(uint8_t tag, uint32_t high, uint32_t low) : 
        ConstantInfo(tag), highBytes(high), lowBytes(low) {}
};
typedef ConstantLong ConstantDouble;


class ConstantNameAndType: public ConstantInfo {
public:
    uint16_t nameIndex;
    uint16_t descriptorIndex;

    ConstantNameAndType(uint16_t name, uint16_t desc) :
        ConstantInfo(CONSTANT_NAMEANDTYPE),
        nameIndex(name), descriptorIndex(desc) {}
};

class ConstantUtf8: public ConstantInfo {
public:
    uint16_t length;
    uint8_t *bytes;

    ConstantUtf8(uint16_t len, uint8_t* _bytes) :
        ConstantInfo(CONSTANT_UTF8),
        length(len), bytes(_bytes) {
    }

    bool equals(string& str) const;
};

class ConstantMethodHandle: public ConstantInfo {
public:
    uint8_t refKind;
    uint16_t refIndex;

    ConstantMethodHandle(uint8_t kind, uint16_t index) :
        ConstantInfo(CONSTANT_METHODHANDLE),
        refKind(kind), refIndex(index) {
    }
};

class ConstantMethodType: public ConstantInfo {
public:
    uint16_t descriptorIndex;
};

class ConstantDynamic: public ConstantInfo {
public:
    uint16_t bootstrapMethodAttrIndex;
    uint16_t nameAndTypeIndex;
};
typedef ConstantDynamic ConstantInvokeDynamic;
typedef ConstantClass ConstantModule;
typedef ConstantClass ConstantPackage;

} // namespace
#endif