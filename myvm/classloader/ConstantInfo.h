#ifndef _CONSTANT_INFO_H_
#define _CONSTANT_INFO_H_
#include "common/types.h"

namespace myvm {

enum ConstantTag {
    Utf8 = 1,
    Integer = 3,
    Float = 4,
    Long = 5,
    Double = 6,
    TagClass = 7,
    String = 8,
    TagFieldRef = 9,
    TagMethodRef = 10,
    InterfaceMethodRef = 11,
    NameAndType = 12,
    MethodHandle = 15,
    MethodType = 16,
    Dynamic = 17,
    InvokeDynamic = 18,
    Module =19,
    Package = 20,
};

struct ConstantInfo {
    uint8_t tag;
};

struct ConstantClass: public ConstantInfo {
    uint8_t nameIndex;
};

struct ConstantFieldRef: public ConstantInfo {
    uint16_t classIndex;
    uint16_t nameAndTypeIndex;
};

typedef ConstantFieldRef ConstantMethodRef;
typedef ConstantFieldRef ConstantInterfaceMethodRef;

struct ConstantString: public ConstantInfo {
    uint16_t stringIndex;
};

struct ConstantInteger: public ConstantInfo {
    uint32_t bytes;
};
typedef ConstantInteger ConstatnFloat;

struct ConstantLong: public ConstantInfo {
    uint32_t highBytes;
    uint32_t lowBytes;
};
typedef ConstantLong ConstantDouble;


struct ConstantNameAndType: public ConstantInfo {
    uint16_t nameIndex;
    uint16_t descriptorIndex;
};

struct ConstantUtf8: public ConstantInfo {
    uint16_t length;
    uint8_t bytes[0];
};

struct ConstantMethodHandle: public ConstantInfo {
    uint8_t refKind;
    uint16_t refIndex;
};

struct ConstantMethodType: public ConstantInfo {
    uint16_t descriptorIndex;
};

struct ConstantDynamic: public ConstantInfo {
    uint16_t bootstrapMethodAttrIndex;
    uint16_t nameAndTypeIndex;
};
typedef ConstantDynamic ConstantInvokeDynamic;
typedef ConstantClass ConstantModule;
typedef ConstantClass ConstantPackage;

} // namespace
#endif