#include "ReturnInstruction.h"

namespace myvm {

void ReturnInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "ReturnInstruction run: clear the stack! " << endl;
}

void ReturnInstruction::run(Frame* frame) {
    cout << INDENTS[frame->getDepth()] << "ReturnInstruction run: clear the stack! " << endl;
}

}