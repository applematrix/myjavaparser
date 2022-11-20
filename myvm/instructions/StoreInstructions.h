#ifndef _STORE_INSTRUCTIONS_H_
#define _STORE_INSTRUCTIONS_H_

#include "Instructions.h"

namespace myvm {

class AStoreInstruction : public Instruction {
public:
    AStoreInstruction() {};
    AStoreInstruction(uint8_t *code);
    AStoreInstruction(uint8_t *code, uint8_t index);
    virtual ~AStoreInstruction() {};
    virtual uint8_t length() { return mOpCode == ASTORE ? 2 : 1;}
    virtual void run(Frame* frame);
private:
    uint8_t mOpCode;
    uint8_t mLocalVariableTableIndex;
};

}

#endif