#ifndef _FRAME_H_
#define _FRAME_H_

#include "LocalVariableTable.h"
#include "OperandStack.h"
#include <vector>

using namespace std;

namespace myvm {

class ConstantInfo;

class Frame {
public:
    Frame(uint32_t stackSize, uint32_t localVariableSize);
    virtual ~Frame();

private:
    LocalVariableTable *mLocalVariableTable;
    OperandStack *mStack;
    std::vector<ConstantInfo*> *mClassConstantPool;
};

}

#endif