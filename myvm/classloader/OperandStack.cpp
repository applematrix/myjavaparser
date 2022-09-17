#include "OperandStack.h"
#include <stdlib.h>

namespace myvm {

OperandStack::OperandStack() {
}

OperandStack::OperandStack(uint32_t maxDepth) {
    mDepth = maxDepth;
}

OperandStack::~OperandStack() {
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
}

void OperandStack::pushInt32(int32_t val) {
    mStack.push((uint32_t)val);
}

uint32_t OperandStack::popUint32() {
    uint32_t value = mStack.top();
    mStack.pop();
    return value;
}

int32_t OperandStack::popInt32() {
    int32_t value = (int32_t)mStack.top();
    mStack.pop();
    return value;
}

uint32_t OperandStack::getSize() {
    return mStack.size();
}

void OperandStack::trimSize(uint32_t size) {
    while(mStack.size() > size) {
        mStack.pop();
    }
}

}