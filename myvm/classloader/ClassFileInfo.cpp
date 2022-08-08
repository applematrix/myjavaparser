/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "ClassFileInfo.h"
#include "FileReader.h"
#include "ConstantFactory.h"
#include "AccessFlag.h"
#include <iostream>

using namespace std;

namespace myvm {

ClassFileInfo::ClassFileInfo() {

}

ClassFileInfo::~ClassFileInfo() {
}

void ClassFileInfo::loadFromFile(const char* path) {
    mFileReader = new FileReader(path);
    
    // read magic number
    int status = mFileReader->readUint32(magic);
    if (status != 0 || magic != JAVA_MAGIC) {
        return;
    }

    status = mFileReader->readUint16(minorVersion);
    if (status != 0) {
        return;
    }
    status = mFileReader->readUint16(majorVersion);
    if (status != 0) {
        return;
    }

    status = loadConstants();
    if (status != 0) {
        return;
    }

    status = mFileReader->readUint16(accessFlags);
    if (status != 0) {
        return;
    }
    string strAccessFlags;
    accessFlagToString(accessFlags, strAccessFlags);
    cout << "Access flags("<< accessFlags << "): " << strAccessFlags << endl;

    status = mFileReader->readUint16(thisClass);
    if (status != 0) {
        return;
    }

    status = mFileReader->readUint16(superClass);
    if (status != 0) {
        return;
    }

    status = loadInterfaces();
    if (status != 0) {
        return;
    }

    status = loadFields();
    if (status != 0) {
        return;
    }

    status = loadMethods();
    if (status != 0) {
        return;
    }

    status = loadAttributes();
    if (status != 0) {
        return;
    }
}

void ClassFileInfo::release() {
    if (mFileReader != nullptr) {
        delete mFileReader;
    }
}

ConstantInfo* ClassFileInfo::getConstantAt(uint16_t index) const {
    // jvms: the index begin from 1; so decrement it
    index--;
    if (index >= mConstantPool.size() || index < 0) {
        return nullptr;
    }
    return mConstantPool.at(index);
}

void ClassFileInfo::printConstantInfo(ConstantInfo *constant) const {
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return;
    }
    ConstantUtf8 *utf8Info = (ConstantUtf8*)constant;
    cout << utf8Info->bytes << endl;
}

void ClassFileInfo::printConstantInfo(uint16_t index) const {
    printConstantInfo(getConstantAt(index));
}

char* ClassFileInfo::getUtf8ConstantName(uint16_t index) const {
    ConstantInfo* constant = getConstantAt(index);
    if (constant == nullptr || constant->tag != CONSTANT_UTF8) {
        return nullptr;
    }
    return (char*)((ConstantUtf8*)constant)->bytes;
}

int ClassFileInfo::loadConstants() {
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
        mConstantPool.push_back(constant);
    }
    return 0;
}

int ClassFileInfo::loadInterfaces() {
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

int ClassFileInfo::loadFields() {
    int status = mFileReader->readUint16(fieldsCount);
    if (status != 0) {
        return status;
    }
    mFields.reserve(fieldsCount);
    for (int i = 0; i < fieldsCount; i++) {
        FieldInfo* field = FieldInfo::loadFromFile(this, mFileReader);
        if (field == nullptr) {
            return -1;
        }
        mFields.push_back(field);
    }
    return 0;
}

int ClassFileInfo::loadMethods() {
    int status = mFileReader->readUint16(methodsCount);
    if (status != 0) {
        return status;
    }

    mMethods.reserve(methodsCount);
    for (int i = 0; i < methodsCount; i++) {
        
        cout << "Load method:" << i << endl;
        MethodInfo* method = MethodInfo::loadFromFile(this, mFileReader);
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

int ClassFileInfo::loadAttributes() {
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

}