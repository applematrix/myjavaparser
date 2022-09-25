#ifndef _ADD_INSTRUCTIONS_H_
#define _ADD_INSTRUCTIONS_H_

#include "Instructions.h"

namespace myvm {

class IAddInstruction : public Instruction {
public:
    IAddInstruction(uint8_t *code);
    virtual ~IAddInstruction() {};
    virtual uint8_t codeLen() { return 1;}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack);
    virtual void run(Frame *frame);
};

}

#endif