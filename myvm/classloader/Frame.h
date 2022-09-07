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
    Frame(ClassFileInfo *clazz, Method *method, uint32_t stackSize, uint32_t localVariableSize, uint16_t depth);
    virtual ~Frame();
    LocalVariableTable* getLocalVariableTable() { return mLocalVariableTable;}
    OperandStack* getStack() { return mStack;}
    ClassFileInfo *getClass() { return mClazz;}
    uint16_t getDepth() { return mCallDepth; }

private:
    ClassFileInfo* mClazz;
    Method* mMethod;
    LocalVariableTable *mLocalVariableTable;
    OperandStack *mStack;
    std::vector<ConstantInfo*> *mClassConstantPool;
    uint16_t mCallDepth;
};

}

#endif