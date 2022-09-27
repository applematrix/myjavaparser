#ifndef _FIELD_INSTRUCTIONS_H_
#define _FIELD_INSTRUCTIONS_H_

#include "Instructions.h"

namespace myvm {

class GetFieldInstruction : public Instruction {
public:
    GetFieldInstruction(uint8_t *code);
    virtual ~GetFieldInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(Frame *frame);
private:
    uint16_t mIndex;
};

class PutFieldInstruction : public Instruction {
public:
    PutFieldInstruction(uint8_t *code);
    virtual ~PutFieldInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(Frame *frame);
private:
    uint16_t mIndex;
};

}



#endif