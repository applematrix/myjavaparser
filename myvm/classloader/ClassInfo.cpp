/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "ClassInfo.h"
#include "FileReader.h"
#include "ConstantFactory.h"
#include "AccessFlag.h"
#include "BootstrapClassLoader.h"
#include "common/utils.h"
#include "common/ClassFileReader.h"
#include "common/JarArchive.h"
#include "common/JarClassFileReader.h"
#include "classloader/Method.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

ClassInfo::ClassInfo() {
    mClassSize = 0;
    mSuperClass = nullptr;
}

ClassInfo::~ClassInfo() {
}

bool ClassInfo::loadFromFile(string& path) {
    mFileReader = new ClassFileReader(path);
    return loadFromFileInternal();
}

bool ClassInfo::loadFromJar(string& jar, string& className) {
    cout << "loadFromJar jar:"<< jar << " , class: " << className << endl;
    JarArchive jarFile;
    jarFile.loadFile(jar);
    if (!jarFile.containsClass(className)) {
        cout << "loadFromJar "<< className << " not found in " << jar << endl;
        return false;
    }
    JarClassFileReader* fileReader = new JarClassFileReader();
    if (!fileReader->open(jar, className)) {
        cout << "loadFromJar open " << jar << " for class " << className << " failed! " << endl;
        return false;
    }
    mFileReader = fileReader;
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

    cout << "--------Constants------" << endl;
    status = loadConstants();
    if (status != 0) {
        return false;
    }
    cout << "----------------------" << endl << endl;

    cout << "-----Access Flags-----" << endl;
    status = mFileReader->readUint16(accessFlags);
    if (status != 0) {
        return false;
    }
    string strAccessFlags;
    accessFlagToString(accessFlags, strAccessFlags);
    cout << "Access flags("<< accessFlags << "): " << strAccessFlags << endl;
    cout << "----------------------" << endl << endl;

    cout << "----this&superClass----" << endl << endl;
    status = mFileReader->readUint16(thisClass);
    if (status != 0) {
        return false;
    }
    status = mFileReader->readUint16(superClass);
    if (status != 0) {
        return false;
    }
    cout << "----------------------" << endl << endl;

    cout << "------Interfaces------" << endl << endl;
    status = loadInterfaces();
    if (status != 0) {
        cout << "Load interfaces failed" << endl;
        return false;
    }
    cout << "----------------------" << endl << endl;

    cout << "--------Fields--------" << endl;
    status = loadFields();
    if (status != 0) {
        cout << "Load fields failed" << endl;
        return false;
    }
    cout << "----------------------" << endl << endl;

    cout << "--------Methods--------" << endl;
    status = loadMethods();
    if (status != 0) {
        cout << "Load methods failed" << endl;
        return false;
    }
    cout << "----------------------" << endl << endl;

    cout << "--------Attributes--------" << endl;
    status = loadAttributes();
    if (status != 0) {
        cout << "Load attributes failed" << endl;
        return false;
    }
    cout << "----------------------" << endl << endl;
    mFileReader->close();

    // resolve the class's runtime data
    return resolve();
}

void ClassInfo::release() {
    if (mFileReader != nullptr) {
        delete mFileReader;
    }
}

ConstantInfo* ClassInfo::getConstantAt(uint16_t index) const {
    // jvms: the index begin from 1; so decrement it
    index--;
    if (index >= mConstantPool.size() || index < 0) {
        return nullptr;
    }
    return mConstantPool.at(index);
}

void ClassInfo::printConstantInfo(ConstantInfo *constant) const {
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return;
    }
    ConstantUtf8 *utf8Info = (ConstantUtf8*)constant;
    cout << utf8Info->bytes << endl;
}

void ClassInfo::printConstantInfo(uint16_t index) const {
    printConstantInfo(getConstantAt(index));
}

char* ClassInfo::getUtf8ConstantName(uint16_t index) const {
    ConstantInfo* constant = getConstantAt(index);
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return nullptr;
    }
    return (char*)((ConstantUtf8*)constant)->bytes;
}

ConstantUtf8* ClassInfo::getUtf8Constant(uint16_t index) const {
    ConstantInfo *constant = getConstantAt(index);
    if (constant == nullptr) {
        return nullptr;
    }
    return (ConstantUtf8*)constant;
}

shared_ptr<FieldInfo> ClassInfo::findField(uint16_t nameIndex, uint16_t descIndex)const {
    for (auto field : mFields) {
        if(field->nameIndex == nameIndex && field->descriptorIndex == descIndex) {
            return field;
        }
    }
    return nullptr;
}

ClassInfo* ClassInfo::getSuperClass() const {
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
        cout << "Load constant #" << i << ": " << constant->typeString() << endl;
        constant->dump(this);
        mConstantPool.push_back(constant);
    }
    cout << "Load constants complete!" << endl << endl;
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

    cout << "Loading "<< methodsCount <<" method ..."<< endl;
    mMethods.reserve(methodsCount);
    for (int i = 0; i < methodsCount; i++) {
        cout << "Load method:" << i << endl;
        Method* method = Method::loadFromFile(this, mFileReader);
        if (method == nullptr) {
            cout << "Load method:" << i << " failed! " << endl;
            return -1;
        }
        mMethods.push_back(method);
        cout << "Load method:" << i << " complete! " << endl << endl;
    }
    cout << endl;
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
        mAttributes.push_back(attr);
    }
    return 0;
}

bool ClassInfo::resolve() {
    for (auto method : mMethods) {
        method->resolve(this);
    }

    ConstantClass* thisClazz = (ConstantClass*)getConstantAt(thisClass);
    ConstantUtf8* classNameUtf8 = (ConstantUtf8*)getConstantAt(thisClazz->nameIndex);
    mClassName = std::string((const char*)classNameUtf8->bytes);
    
    if (superClass == 0) {
        // only the Object class' super class is 0
        if (mClassName.compare(OBJECT_CLASS) != 0) {
            cout << "Error: the class "<< mClassName << " has no parent" << endl;
            return false;
        }
        cout << "Resolve the class "<< mClassName << " without parent " << endl;
        return true;
    }
    ConstantClass* superClazz = (ConstantClass*)getConstantAt(superClass);
    ConstantUtf8* superClassNameUtf8 = (ConstantUtf8*)getConstantAt(superClazz->nameIndex);
    mSuperClassName = std::string((const char*)superClassNameUtf8->bytes);

    BootstrapClassLoader* bootClassLoader = BootstrapClassLoader::getInstance();

    mSuperClass =  bootClassLoader->getClassByName(mSuperClassName);
    if (mSuperClass == nullptr) {
        cout << "Resolve " << mClassName << "\'s super class" << mSuperClassName << endl;
        mSuperClass = bootClassLoader->loadClassFromBootclassPathJar(mSuperClassName);
        if (mSuperClass == nullptr) {
            cout << "Resolve " << mClassName << "\'s super class" << mSuperClassName  << " failed!"<< endl;
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

    // TODO:
    if (mSuperClass != nullptr) {
        mClassSize += mSuperClass->classSize();
    }
    return true;
}

void ClassInfo::invokeMethod() {
}

void ClassInfo::createInstance() {
}

Method* ClassInfo::findMainMethod() {
    for (auto method : mMethods) {
        if (method->isMainEntry()) {
            return method;
        }
    }
    return nullptr;
}

Method* ClassInfo::findMethod(const ConstantNameAndType* nameAndType) {
    for (auto method : mMethods) {
        if (method->match(nameAndType)) {
            return method;
        }
    }
    return nullptr;
}

Method* ClassInfo::findMethod(const ConstantUtf8* methodName, const ConstantUtf8* methodDesc) {
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