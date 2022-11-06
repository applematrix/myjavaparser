#undef LOG_TAG
#define LOG_TAG "IAddInstruction"
#include "../common/Logger.h"

#include "AddInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/ThreadLocalStorage.h"
#include "../classloader/Frame.h"
#include <iostream>
using namespace std;
using namespace myvm;

namespace myvm {

IAddInstruction::IAddInstruction(uint8_t *code) {
}

void IAddInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
}

void IAddInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();

    int32_t operand2 = stack->popInt32();
    int32_t operand1 = stack->popInt32();

    int32_t result = operand1 + operand2;
    stack->pushInt32(result);

    LOGD("%sIAddInstruction run, pop %d"
        " and %d from the stack and push result = %d"
        " back to the stack, current stack size = %d", INDENTS[frame->getDepth()], operand1, operand2, result, stack->getSize());
}

}
