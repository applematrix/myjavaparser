#include "ClassFileInfo.h"
#include "FileReader.h"
#include "ConstantFactory.h"

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
        MethodInfo* method = MethodInfo::loadFromFile(this, mFileReader);
        if (method == nullptr) {
            return -1;
        }
        mMethods.push_back(method);
    }
    return 0;
}

int ClassFileInfo::loadAttributes() {
    int status = mFileReader->readUint16(attributesCount);
    if (status != 0) {
        return -1;
    }

    mAttributes.reserve(attributesCount);
    for (int i = 0; i < attributesCount; i++) {
        // TODO:
        mAttributes.push_back(nullptr);
    }
    return 0;
}

}