#ifndef _RETURN_INSTRUCTIONS_H_
#define _RETURN_INSTRUCTIONS_H_

#include "Instructions.h"

namespace myvm {

class ReturnInstruction : public Instruction {
public:
    ReturnInstruction() {}
    ReturnInstruction(uint8_t *code) {};
    virtual ~ReturnInstruction() {};
    virtual uint8_t codeLen() { return 1;}
    virtual void run(ClassFileInfo* clazz, Method *context, OperandStack *stack);
    virtual void run(Frame* frame);
private:
    bool mPop2 = false;
};

}

#endif