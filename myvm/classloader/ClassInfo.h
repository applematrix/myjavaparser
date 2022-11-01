/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CLASS_INFO_H_
#define _CLASS_INFO_H_

#include <vector>
#include <memory>

#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "AttributeInfo.h"
#include "FileReader.h"
#include "Method.h"

using namespace myvm;
using namespace std;

namespace myvm {

const uint32_t JAVA_MAGIC = 0xCAFEBABE;

class ClassInfo {
public:
    ClassInfo();
    ~ClassInfo();
    bool loadFromFile(string& path);
    bool loadFromJar(string& jar, string& className);
    shared_ptr<ConstantInfo> getConstantAt(uint16_t index) const;
    void printConstantInfo(shared_ptr<ConstantInfo> constant) const;
    void printConstantInfo(uint16_t index) const;
    char* getUtf8ConstantName(uint16_t index) const;
    shared_ptr<ConstantUtf8> getUtf8Constant(uint16_t index) const;
    ClassInfo* getSuperClass() const;
    string getClassName() {
        return mClassName;
    };
    bool resolve();
    shared_ptr<Method> findMainMethod();
    shared_ptr<Method> findMethod(shared_ptr<ConstantNameAndType>& methodInfo);
    shared_ptr<Method> findMethod(shared_ptr<ConstantUtf8>& methodName, shared_ptr<ConstantUtf8>& methodDesc);
    shared_ptr<FieldInfo> findField(uint16_t nameIndex, uint16_t descIndex) const;
    uint32_t classSize() const;
private:
    bool loadFromFileInternal();
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
    std::vector<shared_ptr<ConstantInfo>> mConstantPool;
    std::vector<shared_ptr<FieldInfo>> mFields;
    std::vector<shared_ptr<Method>> mMethods;
    std::vector<AttributeInfo*> mAttributes;
    std::string mClassName;
    std::string mSuperClassName;
    uint32_t mClassSize;

    shared_ptr<FileReader> mFileReader;
    ClassInfo *mSuperClass;
};


}
#endif