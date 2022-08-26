#include "DupInstruction.h"
#include "../classloader/OperandStack.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

void DupInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "Duplicate instance"<< endl;

    uint32_t handle = stack->popUint32();
    stack->pushUint32(handle);
    stack->pushUint32(handle);
}

}