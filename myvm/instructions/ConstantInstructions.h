#ifndef _CONSTANT_INSTRUCTIONS_H_
#define _CONSTANT_INSTRUCTIONS_H_
#include "Instructions.h"

namespace myvm {

class IConstantInstruction : public Instruction {
public:
    IConstantInstruction(uint8_t *code, int32_t value);
    virtual ~IConstantInstruction() {};
    virtual void run(Frame* frame);
private:
    int32_t mValue;
};

}

#endif