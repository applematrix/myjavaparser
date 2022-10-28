/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CLASS_FILE_H_
#define _CLASS_FILE_H_

#include <vector>

#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "AttributeInfo.h"
#include "FileReader.h"
#include "Method.h"

using namespace myvm;

namespace myvm {

const uint32_t JAVA_MAGIC = 0xCAFEBABE;

class ClassInfo {
public:
    ClassInfo();
    ~ClassInfo();
    void loadFromFile(const char* filePath);
    ConstantInfo* getConstantAt(uint16_t index) const;
    void printConstantInfo(ConstantInfo *constant) const;
    void printConstantInfo(uint16_t index) const;
    char* getUtf8ConstantName(uint16_t index) const;
    ConstantUtf8* getUtf8Constant(uint16_t index) const;
    ClassInfo* getSuperClass() const;
    const char* getClassName() {
        return mClassName.c_str();
    };
    void resolve();
    Method* findMainMethod();
    Method* findMethod(const ConstantNameAndType *methodInfo);
    Method* findMethod(const ConstantUtf8* methodName, const ConstantUtf8 *methodDesc);
    FieldInfo* findField(uint16_t nameIndex, uint16_t descIndex) const;
    uint32_t classSize() const;
private:
    int loadConstants();
    int loadInterfaces();
    int loadFields();
    int loadMethods();
    int loadAttributes();
    void release();
    void createInstance();
    
    // test code
    void invokeMethod();
private:
    // JVMS properties
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

    // runtime properties
    uint16_t *mInterfaces;
    std::vector<ConstantInfo*> mConstantPool;
    std::vector<FieldInfo*> mFields;
    std::vector<Method*> mMethods;
    std::vector<AttributeInfo*> mAttributes;
    std::string mClassName;
    std::string mSuperClassName;
    uint32_t mClassSize;

    FileReader *mFileReader;
    ClassInfo *mSuperClass;
};


}
#endif