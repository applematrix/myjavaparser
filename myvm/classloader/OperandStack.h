/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _OPERAND_STACK_H_
#define _OPERAND_STACK_H_

#define STACK_UNIT_SIZE sizeof(uint32_t)

#include <stack>

using namespace std;

namespace myvm {

class ObjectRef;

class OperandStack {
public:
    OperandStack();
    OperandStack(uint32_t maxDepth);
    virtual ~OperandStack();
    void grow(uint16_t growSize);

    void pushUint32(uint32_t val);
    void pushUint64(uint64_t val);
    void pushInt32(int32_t val);
    uint32_t popUint32();
    int32_t popInt32();
    uint64_t popUint64();
    uint32_t pop();
    uint32_t getSize();
    void trimSize(uint32_t size);
private:
    uint32_t mDepth;
    stack<uint32_t> mStack;
};

}

#endif