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

void IConstantInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushInt32(mValue);
    cout << INDENTS[frame->getDepth()] << "IConstant run push " << mValue << " into the stack"
        << ", current stack size =" << stack->getSize()<< endl;
}

}
