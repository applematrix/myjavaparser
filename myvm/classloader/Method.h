/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _METHOD_H_
#define _METHOD_H_

#include "TypeInfo.h"

#include <stdint.h>
#include <vector>
#include <memory>
#include <string>

using namespace std;
using namespace myvm;

namespace myvm {

struct AttributeInfo;
struct CodeAttr;
class ConstantNameAndType;
class ClassInfo;
class FileReader;
class OperandStack;
class LocalVariableTable;
class ConstantUtf8;
class Frame;

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

class Method {
public:
    static Method* loadFromFile(ClassInfo *classFileInfo, FileReader *fileReader);
    
    Method(ClassInfo* owner, uint16_t flags, uint16_t name, uint16_t desc, vector<shared_ptr<AttributeInfo>> *attrs);
    ~Method();
    void invoke(shared_ptr<Frame> frame);
    bool isAbstract();
    bool isStatic();
    bool isPublic();
    bool isPrivate();
    bool isProtected();
    bool isMainEntry();
    bool match(shared_ptr<ConstantNameAndType>& nameAndType);
    bool match(shared_ptr<ConstantUtf8>& methodName, shared_ptr<ConstantUtf8>& methodDesc);
    void resolve(ClassInfo *clazz);
    const string& getName() const;
    const string& getDesc() const;
    OperandStack* getOperandStack();
    LocalVariableTable* getLocalVariableTable();
    ClassInfo* getClass();
    shared_ptr<CodeAttr> getCodeAttr();
    vector<shared_ptr<TypeInfo>> getArgs() {return mArgsList;}
private:
    void resolveParaListAndType();
private:
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t attributeCount;
    vector<shared_ptr<AttributeInfo>> mAttributes;
    vector<shared_ptr<TypeInfo>> mArgsList;
    shared_ptr<TypeInfo> mReturnType;

    OperandStack* mOperandStack;
    LocalVariableTable *mLocalVariables;
    ClassInfo* mOwnerClazz;

    // resolved info
    std::string mName;
    std::string mDescriptor;
    bool mMainMethod;
    bool mConstructor;
    shared_ptr<CodeAttr> mCodeAttr;
    uint16_t mStackSize;
};

}

#endif