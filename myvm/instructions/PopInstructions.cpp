#include "PopInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"

using namespace myvm;

namespace myvm {

void PopInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "PopInstruction run: pop " << (mPop2 ? 2 : 1) << " items from the stack! " << endl;
    // TODO:
    stack->popUint32();
    if (mPop2) {
        stack->popUint32();
    }
}

void PopInstruction::run(Frame* frame) {
    cout << "PopInstruction run: pop " << (mPop2 ? 2 : 1) << " items from the stack! " << endl;

    OperandStack *stack = frame->getStack();
    // TODO:
    stack->popUint32();
    if (mPop2) {
        stack->popUint32();
    }
}

}