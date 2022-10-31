#ifndef _TYPE_INFO_H_
#define _TYPE_INFO_H_

#include "ConstantInfo.h"

#include <stdint.h>
#include <string>
#include <memory>
using namespace std;
using namespace myvm;

namespace myvm {

enum Type {
    TYPE_BYTE,
    TYPE_CHAR,
    TYPE_DOUBLE,
    TYPE_FLOAT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_REF,
    TYPE_SHORT,
    TYPE_BOOLEAN,
    TYPE_ARRAY,
    TYPE_VOID,
};

class TypeInfo {
public:
    TypeInfo(Type type) {this->type = type;}
    virtual ~TypeInfo(){};
    virtual uint8_t descLength() = 0;
    virtual bool doubleUnit() { 
        return type == TYPE_DOUBLE || type == TYPE_LONG;
    }
    //virtual const char* getDesc() = 0;
    static shared_ptr<TypeInfo> parseFrom(const string& desc, uint16_t offset);
    static shared_ptr<TypeInfo> parseFrom(shared_ptr<ConstantUtf8> utf8);
public:
    Type type;
};

class PrimitiveTypeInfo : public TypeInfo {
public:
    PrimitiveTypeInfo(Type type) : TypeInfo(type) {}
    ~PrimitiveTypeInfo(){}
    virtual uint8_t descLength() {return 1;}
};

class ReferenceTypeInfo : public TypeInfo {
public:
    ReferenceTypeInfo(const string& desc);
    ~ReferenceTypeInfo(){}
    virtual uint8_t descLength();
    string classDescriptor;
};

class ArrayTypeInfo : public TypeInfo {
public:
    ArrayTypeInfo(uint8_t dimensions, shared_ptr<TypeInfo> elementType);
    ~ArrayTypeInfo() {};
    virtual uint8_t descLength();

    uint8_t dimensions;
    shared_ptr<TypeInfo> elementType;
};

}
#endif
