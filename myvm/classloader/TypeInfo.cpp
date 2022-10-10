#include "TypeInfo.h"

namespace myvm {

/*static*/ shared_ptr<TypeInfo> TypeInfo::parseFrom(ConstantUtf8 *utf8) {
    string str((const char*)utf8->bytes);
    return parseFrom(str, 0);
}

/*static*/ shared_ptr<TypeInfo> TypeInfo::parseFrom(const string& desc, uint16_t offset) {
    char c = desc.at(offset);
    uint8_t dimensions = 0;
    size_t pos;
    switch (c)
    {
    case 'B':
        return make_shared<PrimitiveTypeInfo>(TYPE_BOOLEAN);
    case 'C':
        return make_shared<PrimitiveTypeInfo>(TYPE_CHAR);
    case 'D':
        return make_shared<PrimitiveTypeInfo>(TYPE_DOUBLE);
    case 'F':
        return make_shared<PrimitiveTypeInfo>(TYPE_FLOAT);
    case 'I':
        return make_shared<PrimitiveTypeInfo>(TYPE_INT);
    case 'J':
        return make_shared<PrimitiveTypeInfo>(TYPE_LONG);
    case 'L':
        pos = desc.find_first_of(";");
        if (pos == string::npos) {
            return nullptr;
        }
        return make_shared<ReferenceTypeInfo>(desc.substr(offset+1, pos - offset -1));
    case 'S':
        return make_shared<PrimitiveTypeInfo>(TYPE_SHORT);
    case 'Z':
        return make_shared<PrimitiveTypeInfo>(TYPE_BOOLEAN);
    case 'V':
        return make_shared<PrimitiveTypeInfo>(TYPE_VOID);
    case '[':
        while (c = desc.at(offset) == '[') {
            offset++;
            dimensions++;
        }
        shared_ptr<TypeInfo> elementType = parseFrom(desc, offset);
        return make_shared<ArrayTypeInfo>(dimensions, elementType);
        return nullptr;
    }
    return nullptr;
}

ReferenceTypeInfo::ReferenceTypeInfo(const string& desc) 
    : TypeInfo(TYPE_REF) {
    classDescriptor = desc;
}

uint8_t ReferenceTypeInfo::descLength() {
    return 1/*'L'*/ + classDescriptor.length() + 1/*';'*/;
}

ArrayTypeInfo::ArrayTypeInfo(uint8_t dim, shared_ptr<TypeInfo> type)
    : TypeInfo(TYPE_REF), dimensions(dim), elementType(type) {
    }

uint8_t ArrayTypeInfo::descLength() {
    return dimensions/*'['s*/ + elementType->descLength();
}



}
