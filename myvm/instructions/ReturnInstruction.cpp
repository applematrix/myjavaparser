#include "ReturnInstruction.h"
#include "../classloader/Frame.h"
#include "../classloader/ThreadLocalStorage.h"
#include "../classloader/OperandStack.h"

using namespace myvm;

namespace myvm {

void ReturnInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->trimSize(frame->getStackReturn());

    cout << INDENTS[frame->getDepth()] << "ReturnInstruction run: clear the stack"
        << ", current stack size =" << stack->getSize()<< endl;
}

////////////////////////////////////
void IReturnInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    int32_t returnValue = stack->popInt32();
    stack->trimSize(frame->getStackReturn());
    stack->pushInt32(returnValue);

    cout << INDENTS[frame->getDepth()] << "IReturnInstruction run: clear the stack of current frame "
        << ", and push " << returnValue << " into the stack for the caller"
        << ", current stack size =" << stack->getSize()<< endl;
}

}