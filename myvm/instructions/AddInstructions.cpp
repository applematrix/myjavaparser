#include "AddInstructions.h"
#include "classloader/OperandStack.h"
#include "classloader/ThreadLocalStorage.h"
#include "classloader/Frame.h"
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
    
    cout << INDENTS[frame->getDepth()] << "IAddInstruction run, pop " << operand1
        << " and "<< operand2 << " from the stack and push result = " << result
        << " back to the stack, current stack size =" << stack->getSize()<< endl;
}

}
