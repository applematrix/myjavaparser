#ifndef _GET_FIELD_INSTRUCTION_H_
#define _GET_FIELD_INSTRUCTION_H_

#include "Instructions.h"

namespace myvm {

class GetFieldInstruction : public Instruction {
public:
    GetFieldInstruction(uint8_t *code);
    virtual ~GetFieldInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(Method *context);
private:
    uint16_t index;
};

}

#endif