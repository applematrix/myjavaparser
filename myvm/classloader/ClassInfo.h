/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CLASS_INFO_H_
#define _CLASS_INFO_H_

#include <memory>
#include <vector>

#include "AttributeInfo.h"
#include "ClassLoader.h"
#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "FileReader.h"
#include "Method.h"

using namespace myvm;
using namespace std;

namespace myvm {
class ClassLoader;

const uint32_t JAVA_MAGIC = 0xCAFEBABE;

class ClassInfo : public enable_shared_from_this<ClassInfo> {
public:
    ClassInfo(const shared_ptr<ClassLoader> &classLoader);
    ~ClassInfo();
    bool loadFromFile(string& path);
    bool loadFromJar(string& jar, string& className);
    ConstantInfo* getConstantAt(uint16_t index) const;
    void printConstantInfo(ConstantInfo *constant) const;
    void printConstantInfo(uint16_t index) const;
    char* getUtf8ConstantName(uint16_t index) const;
    weak_ptr<ClassInfo> getSuperClass() const;
    string getClassName() {
        return mClassName;
    };
    template<class T>
    T* getConstant(uint16_t index) const {
        return dynamic_cast<T*>(getConstantAt(index));
    }

    bool resolve();
    shared_ptr<Method> findMainMethod();
    shared_ptr<Method> findMethod(ConstantNameAndType *methodInfo);
    shared_ptr<Method> findMethod(ConstantUtf8* methodName, ConstantUtf8* methodDesc);
    shared_ptr<FieldInfo> findField(uint16_t nameIndex, uint16_t descIndex) const;
    shared_ptr<FieldInfo> findField(shared_ptr<ConstantNameAndType>& fieldInfo) const;
    uint32_t classSize() const;
    void setClassLoader(shared_ptr<ClassLoader> &classLoader);
    weak_ptr<ClassLoader> getClassLoader() const;
    void invokeClassInitMethod();
private:
    void createInstance();
    void evalClassSize();
    bool linkClasses();
    bool loadFromFileInternal();
    int loadConstants();
    int loadInterfaces();
    int loadFields();
    int loadMethods();
    int loadAttributes();
    void release();
    
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
    //std::vector<shared_ptr<ConstantInfo>> mConstantPool;
    vector<unique_ptr<ConstantInfo>> mConstantPool;
    //ConstantInfo** mConstantPool = nullptr;
    uint16_t mConstantPoolSize = 0;
    std::vector<shared_ptr<FieldInfo>> mFields;
    std::vector<shared_ptr<Method>> mMethods;
    std::vector<shared_ptr<AttributeInfo>> mAttributes;
    std::string mClassName;
    std::string mSuperClassName;
    uint32_t mClassSize = 0;

    shared_ptr<FileReader> mFileReader;
    weak_ptr<ClassInfo> mSuperClass;
    weak_ptr<ClassLoader> mClassLoader;
    list<string> mPendingLinkClasses;
};


}
#endif