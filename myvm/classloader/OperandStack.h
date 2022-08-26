/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _OPERAND_STACK_H_
#define _OPERAND_STACK_H_

#include <stack>

using namespace std;

namespace myvm {

class ObjectRef;

class OperandStack {
public:
    OperandStack(uint32_t stackSize);
    virtual ~OperandStack();
    void pushUint8(uint8_t val);
    void pushUint16(uint16_t val);
    void pushUint32(uint32_t val);
    void pushObjecRef(ObjectRef *reference);
    void push(uint32_t val);
    uint8_t popUint8();
    uint16_t popUint16();
    uint32_t popUint32();
    ObjectRef *popObjecRef();
    uint32_t pop();
    void reset();
private:
    uint8_t *mBuffer;
    uint32_t mSize;
    uint32_t mCurPos;
    uint32_t mDepth;
};

}

#endif