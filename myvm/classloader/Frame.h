#ifndef _FRAME_H_
#define _FRAME_H_

#include "LocalVariableTable.h"
#include "OperandStack.h"
#include <vector>
#include <memory>

using namespace std;

namespace myvm {

class ConstantInfo;
class ClassInfo;
class Method;

class Frame {
public:
    Frame(shared_ptr<Method> method, uint32_t localVariableSize, uint16_t depth);
    virtual ~Frame();
    shared_ptr<LocalVariableTable> getLocalVariableTable() { return mLocalVariableTable;}
    ClassInfo *getClass() { return mClazz;}
    shared_ptr<Method> getMethod() { return mMethod; }
    uint16_t getDepth() { return mCallDepth; }
    uint16_t getStackReturn();

private:
    ClassInfo* mClazz;
    shared_ptr<Method> mMethod;
    shared_ptr<LocalVariableTable> mLocalVariableTable;
    std::vector<ConstantInfo*> *mClassConstantPool;
    uint16_t mCallDepth;
    uint32_t mStackReturnPos;
};

}

#endif