#ifndef _METHOD_H_
#define _METHOD_H_

#include <stdint.h>
#include <vector>
#include <string>

using namespace std;

namespace myvm {

struct AttributeInfo;
struct CodeAttr;
class ClassFileInfo;
class FileReader;
class OperandStack;
class LocalVariableTable;

enum MethodAccessFlag {
    METHOD_ACC_PUBLIC = 0x0001,
    METHOD_ACC_PRIVATE = 0x0002,
    METHOD_ACC_PROTECTED = 0x0004,
    METHOD_ACC_STATIC = 0x0008,
    METHOD_ACC_FINAL = 0x0010,
    METHOD_ACC_SYNCHRONIZED = 0x0020,
    METHOD_ACC_BRIDGE = 0x0040,
    METHOD_ACC_VARARGS = 0x0080,
    METHOD_ACC_NATIVE = 0x0100,
    METHOD_ACC_ABSTRACT = 0x0400,
    METHOD_ACC_STRICT = 0x0800,
    METHOD_ACC_SYNTHETIC = 0x1000,
};

struct Method {
public:
    static Method* loadFromFile(ClassFileInfo *classFileInfo, FileReader *fileReader);
    
    Method(uint16_t flags, uint16_t name, uint16_t desc, vector<AttributeInfo*> *attrs);
    ~Method();
    void invoke(ClassFileInfo *clazz);
    bool isAbstract();
    bool isStatic();
    bool isPublic();
    bool isPrivate();
    bool isProtected();
    bool isMainEntry();
    void resolve(ClassFileInfo *clazz);
private:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    vector<AttributeInfo*> mAttributes;

    OperandStack* mOperandStack;
    LocalVariableTable *mLocalVariables;

    // resolved info
    std::string mName;
    std::string mDescriptor;
    bool mMainMethod;
    bool mConstructor;
    CodeAttr* mCodeAttr;
    uint16_t mStackSize;
};

}

#endif