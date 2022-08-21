#ifndef _METHOD_H_
#define _METHOD_H_

#include <stdint.h>
#include <vector>

using namespace std;

namespace myvm {

struct AttributeInfo;
class ClassFileInfo;
class FileReader;
class OperandStack;
class LocalVariableTable;

struct Method {
public:
    static Method* loadFromFile(ClassFileInfo *classFileInfo, FileReader *fileReader);
    
    Method(uint16_t flags, uint16_t name, uint16_t desc, vector<AttributeInfo*> *attrs);
    ~Method();
    void invoke();
    bool isAbstract();
    bool isInterface();
private:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    vector<AttributeInfo*> mAttributes;

    OperandStack* mOperandStack;
    LocalVariableTable *mLocalVariables;
};

}

#endif