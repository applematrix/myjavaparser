/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _NEW_INSTRCTIONS_H_
#define _NEW_INSTRCTIONS_H_

#include "Instructions.h"
#include "../classloader/Method.h"

using namespace myvm;

namespace myvm {

class ClassFileInfo;

class NewInstruction : public Instruction {
public:
    NewInstruction() {};
    NewInstruction(uint8_t *code);
    virtual ~NewInstruction() {};
    virtual uint8_t codeLen() { return 3;}
    virtual void run(Frame *frame);
private:
    uint16_t mIndex;
};


}

#endif
