/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#undef LOG_TAG
#define LOG_TAG "ClassInfo"

#include "ClassInfo.h"
#include "FileReader.h"
#include "ConstantFactory.h"
#include "AccessFlag.h"
#include "BootstrapClassLoader.h"
#include "../common/utils.h"
#include "../common/ClassFileReader.h"
#include "../common/JarArchive.h"
#include "../common/JarClassFileReader.h"
#include "../common/Logger.h"
#include "Method.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

ClassInfo::ClassInfo() {
    mClassSize = 0;
}

ClassInfo::~ClassInfo() {
}

bool ClassInfo::loadFromFile(string& path) {
    mFileReader = make_shared<ClassFileReader>(path);
    return loadFromFileInternal();
}

bool ClassInfo::loadFromJar(string& jar, string& className) {
    LOGI("loadFromJar jar: %s, class: %s", jar.c_str(), className.c_str());
    JarArchive jarFile;
    jarFile.loadFile(jar);
    if (!jarFile.containsClass(className)) {
        LOGW("loadFromJar %s not found in %s", className.c_str(), jar.c_str());
        return false;
    }
    JarClassFileReader* fileReader = new JarClassFileReader();
    if (!fileReader->open(jar, className)) {
        LOGW("loadFromJar open: %s for class: %s failed!", jar.c_str(), className.c_str());
        return false;
    }
    mFileReader = shared_ptr<FileReader>(fileReader);
    return loadFromFileInternal();
}

bool ClassInfo::loadFromFileInternal() {
    // read magic number
    int status = mFileReader->readUint32(magic);
    if (status != 0 || magic != JAVA_MAGIC) {
        return false;
    }

    status = mFileReader->readUint16(minorVersion);
    if (status != 0) {
        return false;
    }
    status = mFileReader->readUint16(majorVersion);
    if (status != 0) {
        return false;
    }

    LOGI("--------Constants------");
    status = loadConstants();
    if (status != 0) {
        return false;
    }
    LOGI("----------------------");

    LOGI("-----Access Flags-----");
    status = mFileReader->readUint16(accessFlags);
    if (status != 0) {
        return false;
    }
    string strAccessFlags;
    accessFlagToString(accessFlags, strAccessFlags);
    LOGI("Access flags(%d): %s", accessFlags, strAccessFlags.c_str());
    LOGI("----------------------");

    LOGI("----this&superClass----");
    status = mFileReader->readUint16(thisClass);
    if (status != 0) {
        return false;
    }
    status = mFileReader->readUint16(superClass);
    if (status != 0) {
        return false;
    }
    LOGI("----------------------");

    LOGI("------Interfaces------");
    status = loadInterfaces();
    if (status != 0) {
        LOGW("Load interfaces failed");
        return false;
    }
    LOGI("----------------------");

    LOGI("--------Fields--------");
    status = loadFields();
    if (status != 0) {
        LOGW("Load fields failed");
        return false;
    }
    LOGI("----------------------");

    LOGI("--------Methods--------");
    status = loadMethods();
    if (status != 0) {
        LOGW("Load methods failed");
        return false;
    }
    LOGI("----------------------");

    LOGI( "--------Attributes--------");
    status = loadAttributes();
    if (status != 0) {
        LOGW("Load attributes failed");
        return false;
    }
    LOGI("----------------------");
    mFileReader->close();

    // resolve the class's runtime data
    return resolve();
}

void ClassInfo::release() {
}

shared_ptr<ConstantInfo> ClassInfo::getConstantAt(uint16_t index) const {
    // jvms: the index begin from 1; so decrement it
    index--;
    if (index >= mConstantPool.size() || index < 0) {
        return nullptr;
    }
    return mConstantPool.at(index);
}

void ClassInfo::printConstantInfo(shared_ptr<ConstantInfo> constant) const {
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return;
    }
    auto utf8Info = dynamic_pointer_cast<ConstantUtf8>(constant);
    LOGD("Constant info:%s", utf8Info->bytes);
}

void ClassInfo::printConstantInfo(uint16_t index) const {
    printConstantInfo(getConstantAt(index));
}

char* ClassInfo::getUtf8ConstantName(uint16_t index) const {
    shared_ptr<ConstantInfo> constant = getConstantAt(index);
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return nullptr;
    }
    auto utf8 = dynamic_pointer_cast<ConstantUtf8>(constant);
    return (char*)utf8->bytes;
}

shared_ptr<FieldInfo> ClassInfo::findField(uint16_t nameIndex, uint16_t descIndex)const {
    for (auto field : mFields) {
        if(field->match(nameIndex, descIndex)) {
            return field;
        }
    }
    return nullptr;
}

shared_ptr<FieldInfo> ClassInfo::findField(shared_ptr<ConstantNameAndType>& fieldInfo) const {
    for (auto field : mFields) {
        if(field->match(fieldInfo)) {
            return field;
        }
    }
    return nullptr;
}

weak_ptr<ClassInfo> ClassInfo::getSuperClass() const {
    return mSuperClass;
}

int ClassInfo::loadConstants() {
    int status = mFileReader->readUint16(constantPoolSize);
    if (status != 0) {
        return -1;
    }
    if (constantPoolSize <= 0) {
        return -1;
    }
    mConstantPool.reserve(constantPoolSize - 1);
    for (int i = 0; i < constantPoolSize - 1; i++) {
        ConstantInfo* constant = ConstantFactory::loadFromFile(mFileReader);
        if (constant == nullptr) {
            return -1;
        }
        LOGI("Load constant #%d: %s", i, constant->typeString());
        constant->dump(this);
        mConstantPool.push_back(shared_ptr<ConstantInfo>(constant));
    }
    LOGI("Load constants complete!");
    return 0;
}

int ClassInfo::loadInterfaces() {
    int status = mFileReader->readUint16(interfacesCount);
    if (status != 0) {
        return -1;
    }
    if (interfacesCount <= 0) {
        return 0;
    }
    mInterfaces = new uint16_t[interfacesCount];
    status = mFileReader->read(mInterfaces, interfacesCount*sizeof(uint16_t));
    return status;
}

int ClassInfo::loadFields() {
    int status = mFileReader->readUint16(fieldsCount);
    if (status != 0) {
        return status;
    }
    uint32_t fieldMemorySize = 0;
    mFields.reserve(fieldsCount);
    for (int i = 0; i < fieldsCount; i++) {
        FieldInfo* field = FieldInfo::loadFromFile(this, mFileReader);
        if (field == nullptr) {
            return -1;
        }
        mFields.push_back(shared_ptr<FieldInfo>(field));
    }
    return 0;
}

int ClassInfo::loadMethods() {
    int status = mFileReader->readUint16(methodsCount);
    if (status != 0) {
        return status;
    }

    LOGI("Loading %d methods ...", methodsCount);
    mMethods.reserve(methodsCount);
    for (int i = 0; i < methodsCount; i++) {
        LOGI("Load method:", i);
        Method* method = Method::loadFromFile(this, mFileReader);
        if (method == nullptr) {
            LOGW("Load method #%d complete!", i);
            return -1;
        }
        mMethods.push_back(shared_ptr<Method>(method));
        LOGI("Load method #%d complete!", i);
    }
    return 0;
}

int ClassInfo::loadAttributes() {
    int status = mFileReader->readUint16(attributesCount);
    if (status != 0) {
        return -1;
    }

    mAttributes.reserve(attributesCount);
    for (int i = 0; i < attributesCount; i++) {
        AttributeInfo *attr = AttributeFactory::loadFromFile(this, mFileReader);
        mAttributes.push_back(shared_ptr<AttributeInfo>(attr));
    }
    return 0;
}

void ClassInfo::loadClassesInConstantPool() {

}

bool ClassInfo::resolve() {
    for (auto method : mMethods) {
        method->resolve(this);
    }

    auto thisClazz = getConstant<ConstantClass>(thisClass);
    auto classNameUtf8 = getConstant<ConstantUtf8>(thisClazz->nameIndex);
    mClassName = std::string((const char*)classNameUtf8->bytes);
    
    if (superClass == 0) {
        // only the Object class' super class is 0
        if (mClassName.compare(OBJECT_CLASS) != 0) {
            LOGW("Error: the class:%s has no parent", mClassName.c_str());
            return false;
        }
        LOGI("Resolve the class :%s without parent", mClassName.c_str());
        return true;
    }
    auto superClazz = getConstant<ConstantClass>(superClass);
    auto superClassNameUtf8 = getConstant<ConstantUtf8>(superClazz->nameIndex);
    mSuperClassName = std::string((const char*)superClassNameUtf8->bytes);

    BootstrapClassLoader* bootClassLoader = BootstrapClassLoader::getInstance();

    auto superClass =  bootClassLoader->getClassByName(mSuperClassName);
    if (superClass == nullptr) {
        LOGI("Resolve the class :%s \'s super class:", mClassName.c_str(), mSuperClassName.c_str());
        if (bootClassLoader->loadClassFromBootclassPathJar(mSuperClassName) == nullptr) {
            LOGW("Resolve the class :%s \'s super class failed!", mClassName.c_str(), mSuperClassName.c_str());
            return false;
        }
    }

    // resolve the class size
    mClassSize = 0;
    uint32_t offset = 0;
    for (auto field : mFields) {
        field->resolve();
        mClassSize += field->getType()->doubleUnit() ? sizeof(uint64_t) : sizeof(uint32_t);
        field->updateOffset(offset);
        offset += field->getType()->doubleUnit() ? 2 : 1;
    }

    if (superClass != nullptr) {
        mClassSize += superClass->classSize();
    }
    mSuperClass = superClass;
    return true;
}

void ClassInfo::invokeMethod() {
}

void ClassInfo::createInstance() {
}

shared_ptr<Method> ClassInfo::findMainMethod() {
    for (auto method : mMethods) {
        if (method->isMainEntry()) {
            return method;
        }
    }
    return nullptr;
}

shared_ptr<Method> ClassInfo::findMethod(shared_ptr<ConstantNameAndType>& nameAndType) {
    for (auto method : mMethods) {
        if (method->match(nameAndType)) {
            return method;
        }
    }
    return nullptr;
}

shared_ptr<Method> ClassInfo::findMethod(shared_ptr<ConstantUtf8>& methodName, shared_ptr<ConstantUtf8>& methodDesc) {
    for (auto method : mMethods) {
        if (method->match(methodName, methodDesc)) {
            return method;
        }
    }
    return nullptr;
}

uint32_t ClassInfo::classSize() const {
    return mClassSize;
}

}
