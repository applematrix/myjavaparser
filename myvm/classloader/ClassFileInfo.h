#ifndef _CLASS_FILE_H_
#define _CLASS_FILE_H_

#include "types.h"
#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"
#include "AttributeInfo.h"

namespace myvm {

class ClassFileInfo {
public:
    ClassFileInfo();
    ~ClassFileInfo();
private:
    uint32_t magic;
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint16_t constantPoolSize;
    uint16_t accessFlags;
    uint16_t thisClass;
    uint16_t superClass;
    uint16_t interfacesCount;
    uint16_t fieldsCount;
    uint16_t methodsCount;
    uint16_t attributesCount;

    ConstantInfo *mConstantPool;
    FieldInfo *mFields;
    MethodInfo *mMethods;
    AttributeInfo *mAttributes;
};


}
#endif