#ifndef _FRAME_H_
#define _FRAME_H_

#include "LocalVariableTable.h"
#include "OperandStack.h"
#include <vector>

using namespace std;

namespace myvm {

class ConstantInfo;
class ClassFileInfo;
class Method;

class Frame {
public:
    Frame(ClassFileInfo *clazz, Method *method, uint32_t stackSize, uint32_t localVariableSize);
    virtual ~Frame();
    LocalVariableTable* getLocalVariableTable() { return mLocalVariableTable;}
    OperandStack* getStack() { return mStack;}
    ClassFileInfo *getClass() { return mClazz;}

private:
    ClassFileInfo* mClazz;
    Method* mMethod;
    LocalVariableTable *mLocalVariableTable;
    OperandStack *mStack;
    std::vector<ConstantInfo*> *mClassConstantPool;
};

}

#endif