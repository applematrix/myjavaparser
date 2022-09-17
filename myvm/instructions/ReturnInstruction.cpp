#include "ReturnInstruction.h"
#include "../classloader/Frame.h"
#include "../classloader/ThreadLocalStorage.h"
#include "../classloader/OperandStack.h"

using namespace myvm;

namespace myvm {

void ReturnInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "ReturnInstruction run: clear the stack! " << endl;
}

void ReturnInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();
    curStack->trimSize(frame->getStackReturn());
    cout << INDENTS[frame->getDepth()] << "ReturnInstruction run: clear the stack! " << endl;

}

}