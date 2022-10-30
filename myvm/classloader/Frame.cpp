#include "Frame.h"
#include "Method.h"
#include "LocalVariableTable.h"
#include "classloader/ThreadLocalStorage.h"
#include "classloader/OperandStack.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

Frame::Frame(shared_ptr<Method> method, uint32_t localVariableSize, uint16_t depth) {
    mLocalVariableTable = make_shared<LocalVariableTable>(method, localVariableSize);
    mClazz = method->getClass();
    mMethod = method;
    mCallDepth = depth;

    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();
    mStackReturnPos = curStack->getSize();
}

Frame::~Frame() {
}

uint16_t Frame::getStackReturn() {
    return mStackReturnPos;
}

}