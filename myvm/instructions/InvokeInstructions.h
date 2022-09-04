#ifndef _INVOKE_INSTRUCTIONS_H_
#define _INVOKE_INSTRUCTIONS_H_

#include "Instructions.h"

using namespace myvm;

namespace myvm {

class InvokeSpecialInstruction : public Instruction {
public:
    InvokeSpecialInstruction(uint8_t *code);
    virtual ~InvokeSpecialInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack);
    virtual void run(Frame *frame);
private:
    uint8_t mIndex;
};


class InvokeVirtualInstruction : public Instruction {
public:
    InvokeVirtualInstruction(uint8_t *code);
    virtual ~InvokeVirtualInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack);
    virtual void run(Frame *frame);
private:
    uint8_t mIndex;
};

}

#endif