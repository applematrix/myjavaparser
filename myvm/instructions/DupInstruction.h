#ifndef _DUP_INSTRUCTION_H_
#define _DUP_INSTRUCTION_H_

#include "Instructions.h"
using namespace myvm;

namespace myvm {

class DupInstruction : public Instruction {
public:
    DupInstruction() {};
    virtual ~DupInstruction() {};
    virtual uint8_t codeLen() { return 1;}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack);
    virtual void run(Frame *frame);
};

}

#endif