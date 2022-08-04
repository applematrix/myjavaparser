#ifndef _CLASS_FILE_H_
#define _CLASS_FILE_H_

#include <vector>

#include "types.h"
#include "ConstantInfo.h"
#include "FieldInfo.h"
#include "AttributeInfo.h"
#include "FileReader.h"

using namespace myvm;

namespace myvm {

const uint32_t JAVA_MAGIC = 0xCAFEBABE;

class ClassFileInfo {
public:
    ClassFileInfo();
    ~ClassFileInfo();
    void loadFromFile(const char* filePath);
    ConstantInfo* getConstantAt(uint16_t index) const;
    void printConstantInfo(ConstantInfo *constant) const;
    void printConstantInfo(uint16_t index) const;
    char* getUtf8ConstantName(uint16_t index) const;
private:
    int loadConstants();
    int loadInterfaces();
    int loadFields();
    int loadMethods();
    int loadAttributes();
    void release();
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

    uint16_t *mInterfaces;
    std::vector<ConstantInfo*> mConstantPool;
    std::vector<FieldInfo*> mFields;
    std::vector<MethodInfo*> mMethods;
    std::vector<AttributeInfo*> mAttributes;

    FileReader *mFileReader;
};


}
#endif