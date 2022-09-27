/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _LOAD_INSTRCTIONS_H_
#define _LOAD_INSTRCTIONS_H_

#include "Instructions.h"
#include "../classloader/Method.h"

using namespace myvm;

namespace myvm {
class OperandStack;

class IloadInstruction : public Instruction {
public:
    IloadInstruction() {};
    virtual ~IloadInstruction() {};
    virtual uint8_t codeLen() { return 2;}
    virtual void run(Frame* frame);
private:
    uint8_t mIndex;
};

class Iload0Instruction : public Instruction {
public:
    Iload0Instruction() {};
    virtual ~Iload0Instruction() {};
    virtual uint8_t codeLen() { return 1;}
    virtual void run(Frame* frame);
};

class Iload1Instruction : public Instruction {
public:
    Iload1Instruction() {};
    virtual ~Iload1Instruction() {};
    virtual uint8_t codeLen() { return 1;}
    virtual void run(Frame* frame);
};

class AloadInstruction : public Instruction {
public:
    AloadInstruction() {};
    AloadInstruction(uint8_t *code);
    AloadInstruction(uint8_t *code, uint8_t index);
    virtual ~AloadInstruction() {};
    virtual uint8_t codeLen() { return mOpCode == ALOAD ? 2 : 1;}
    virtual void run(Frame* frame);
private:
    uint8_t mOpCode;
    uint8_t mLocalVariableTableIndex;
};

}

#endif