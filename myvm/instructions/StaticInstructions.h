#ifndef _STATIC_INSTRUCTIONS_H_
#define _STATIC_INSTRUCTIONS_H_

#include "Instructions.h"

namespace myvm {

class GetStaticInstruction : public Instruction {
public:
    GetStaticInstruction() {}
    GetStaticInstruction(uint8_t *code);
    virtual ~GetStaticInstruction() {};
    virtual uint8_t length() { return 3;}
    virtual void run(Frame* frame);
private:
    uint16_t mIndex;
};

}

#endif