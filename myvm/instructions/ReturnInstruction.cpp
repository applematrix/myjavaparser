#undef LOG_TAG
#define LOG_TAG "ReturnInstruction"
#include "common/Logger.h"

#include "ReturnInstruction.h"
#include "../classloader/Frame.h"
#include "../classloader/ThreadLocalStorage.h"
#include "../classloader/OperandStack.h"

using namespace myvm;

namespace myvm {

void ReturnInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->trimSize(frame->getStackReturn());
    
    LOGD("%sReturnInstruction run: clear the stack"
            ", current stack size =%d", INDENTS[frame->getDepth()], stack->getSize());
}

////////////////////////////////////
void IReturnInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    int32_t returnValue = stack->popInt32();
    stack->trimSize(frame->getStackReturn());
    stack->pushInt32(returnValue);
    
    LOGD("%sReturnInstruction run: clear the stack of current frame "
            ", and push %d into the stack for the caller"
            ", current stack size =%d", INDENTS[frame->getDepth()], returnValue, stack->getSize());
}

}
