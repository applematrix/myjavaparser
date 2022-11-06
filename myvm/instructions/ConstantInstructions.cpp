#undef LOG_TAG
#define LOG_TAG "IConstantInstruction"
#include "common/Logger.h"

#include "ConstantInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/ThreadLocalStorage.h"
#include <iostream>
using namespace std;
using namespace myvm;

namespace myvm {

IConstantInstruction::IConstantInstruction(uint8_t *code, int32_t value) {
    mValue = value;
}

void IConstantInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushInt32(mValue);
    LOGD("%IConstantInstruction run, push %d"
        " into the stack, current stack size = %d", INDENTS[frame->getDepth()], mValue, stack->getSize());
}

}
