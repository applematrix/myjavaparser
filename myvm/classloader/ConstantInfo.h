#ifndef _CONSTANT_INFO_H_
#define _CONSTANT_INFO_H_
#include <stdint.h>

namespace myvm {

enum ConstantTag {
    Utf8 = 1,
    Integer = 3,
    Float = 4,
    Long = 5,
    Double = 6,
    Class = 7,
    String = 8,
    FieldRef = 9,
    MethodRef = 10,
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

    ConstantInfo(uint8_t _tag): tag(_tag) {}
};

struct ConstantClass: public ConstantInfo {
    uint16_t nameIndex;

    ConstantClass(uint8_t tag, uint16_t index) :
        ConstantInfo(tag), nameIndex(index) {}
};

struct ConstantRef: public ConstantInfo {
    uint16_t classIndex;
    uint16_t nameAndTypeIndex;

    ConstantRef(uint8_t tag, uint16_t index, uint16_t nameAndType) : 
        ConstantInfo(tag), classIndex(index), nameAndTypeIndex(nameAndType) {}
};

typedef ConstantRef ConstantFieldRef;
typedef ConstantFieldRef ConstantMethodRef;
typedef ConstantFieldRef ConstantInterfaceMethodRef;

struct ConstantString: public ConstantInfo {
    uint16_t stringIndex;
    ConstantString(uint16_t index) : 
        ConstantInfo(ConstantTag::String), stringIndex(index) {}
};

struct ConstantInteger: public ConstantInfo {
    uint32_t bytes;
    ConstantInteger(uint32_t _bytes) : 
        ConstantInfo(ConstantTag::Integer), bytes(_bytes) {}
    
    ConstantInteger(uint8_t tag, uint32_t _bytes) : 
        ConstantInfo(tag), bytes(_bytes) {}
};
typedef ConstantInteger ConstantFloat;

struct ConstantLong: public ConstantInfo {
    uint32_t highBytes;
    uint32_t lowBytes;

    ConstantLong(uint8_t tag, uint32_t high, uint32_t low) : 
        ConstantInfo(tag), highBytes(high), lowBytes(low) {}
};
typedef ConstantLong ConstantDouble;


struct ConstantNameAndType: public ConstantInfo {
    uint16_t nameIndex;
    uint16_t descriptorIndex;

    ConstantNameAndType(uint16_t name, uint16_t desc) :
        ConstantInfo(ConstantTag::NameAndType),
        nameIndex(name), descriptorIndex(desc) {}
};

struct ConstantUtf8: public ConstantInfo {
    uint16_t length;
    uint8_t *bytes;

    ConstantUtf8(uint16_t len, uint8_t* _bytes) :
        ConstantInfo(ConstantTag::Utf8),
        length(len), bytes(_bytes) {
    }
};

struct ConstantMethodHandle: public ConstantInfo {
    uint8_t refKind;
    uint16_t refIndex;

    ConstantMethodHandle(uint8_t kind, uint16_t index) :
        ConstantInfo(ConstantTag::MethodHandle),
        refKind(kind), refIndex(index) {
    }
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