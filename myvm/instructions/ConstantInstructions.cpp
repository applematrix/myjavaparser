#include "ConstantInstructions.h"
#include "..\classloader\OperandStack.h"
#include "..\classloader\ThreadLocalStorage.h"
#include <iostream>
using namespace std;
using namespace myvm;

namespace myvm {

IConstantInstruction::IConstantInstruction(uint8_t *code, int32_t value) {
    mValue = value;
}

void IConstantInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "IConstant run push " << mValue << " into the stack! " << endl;
    stack->pushInt32(mValue);
}

void IConstantInstruction::run(Frame* frame) {
    cout << INDENTS[frame->getDepth()] << "IConstant run push " << mValue << " into the stack! " << endl;

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushInt32(mValue);
}

}
