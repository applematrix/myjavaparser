#include "OperandStack.h"
#include <stdlib.h>

namespace myvm {

const uint32_t GROW_STEP = 64;

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

void OperandStack::pushUint8(uint8_t val){
    *mBuffer = val;
    mCurPos += sizeof(val);
}

void OperandStack::pushUint16(uint16_t val) {
    *mBuffer = val;
    mCurPos += sizeof(val);
}

void OperandStack::pushUint32(uint32_t val) {
    *mBuffer = val;
    mCurPos += sizeof(val);
}

void OperandStack::pushInt32(int32_t val) {
    *mBuffer = val;
    mCurPos += sizeof(val);
}

void OperandStack::push(uint32_t val) {
    *mBuffer = val;
    mCurPos += sizeof(val);
}

uint8_t OperandStack::popUint8() {
    if (mCurPos < sizeof(uint8_t)) {
        return 0; // TODO:
    }
    mCurPos -= sizeof(uint8_t);
    return *((uint8_t *)mCurPos);
}

uint16_t OperandStack::popUint16() {
    if (mCurPos < sizeof(uint16_t)) {
        return 0; // TODO:
    }
    mCurPos -= sizeof(uint16_t);
    return *((uint16_t *)mCurPos);
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

uint32_t OperandStack::pop() {
    if (mCurPos < sizeof(uint32_t)) {
        return 0; // TODO:
    }
    mCurPos -= sizeof(uint32_t);
    return *((uint32_t*)(mBuffer + mCurPos));
}

void OperandStack::reset() {
    mCurPos = 0;
}

void OperandStack::pushObjecRef(ObjectRef *reference) {
    memcpy(mBuffer, &reference, sizeof(ObjectRef*));
    mCurPos += sizeof(ObjectRef*);
}

ObjectRef* OperandStack::popObjecRef() {
    if (mCurPos < sizeof(ObjectRef*)) {
        return nullptr; // TODO:
    }
    mCurPos -= sizeof(ObjectRef*);
    ObjectRef* reference = (ObjectRef*)(*mBuffer);
    return reference;
}

}