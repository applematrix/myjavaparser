#include "OperandStack.h"
#include <stdlib.h>

namespace myvm {

const uint32_t GROW_STEP = 64;

OperandStack::OperandStack() {
    mBuffer = nullptr;
    mMaxDepth = 0;
    mCurPos = 0;
}

OperandStack::OperandStack(uint32_t maxDepth) {
    mBuffer = (uint8_t *)malloc(maxDepth * STACK_UNIT_SIZE);
    mMaxDepth = maxDepth;
    mCurPos = 0;
}

OperandStack::~OperandStack() {
    if (mBuffer != nullptr) {
        free(mBuffer);
    }
}

void OperandStack::pushUint32(uint32_t value) {
    mStack.push(value);
}

uint32_t OperandStack::pop() {
    uint32_t value = mStack.top();
    mStack.pop();
    return value;
}

void OperandStack::pushUint64(uint64_t val) {
    uint32_t low = (val & 0xffffffff);
    mStack.push(low);
    uint32_t high = ((val>>32) & 0xffffffff);
    mStack.push(high);
}

uint64_t OperandStack::popUint64() {
    uint64_t high = mStack.top();
    mStack.pop();
    uint32_t low = mStack.top();
    mStack.pop();

    return (high << 32 | low);
}

void OperandStack::grow(uint16_t growSize) {
    mBuffer = (uint8_t *)realloc(mBuffer, growSize * STACK_UNIT_SIZE);
}

void OperandStack::pushInt32(int32_t val) {
    mStack.push((uint32_t)val);
}

uint32_t OperandStack::popUint32() {
    if (mCurPos < sizeof(uint32_t)) {
        return 0; // TODO:
    }
    mCurPos -= sizeof(uint32_t);
    return *((uint32_t *)(mBuffer + mCurPos));
}

int32_t OperandStack::popInt32() {
    if (mCurPos < sizeof(int32_t)) {
        return 0; // TODO:
    }
    mCurPos -= sizeof(int32_t);
    return *((int32_t *)(mBuffer + mCurPos));
}


void OperandStack::reset() {
    mCurPos = 0;
}

}