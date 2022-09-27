#include "PopInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"

using namespace myvm;

namespace myvm {

void PopInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    // TODO:
    stack->popUint32();
    cout << INDENTS[frame->getDepth()] << "PopInstruction run: pop "
        << (mPop2 ? 2 : 1) << " items from the stack! " << endl;
    if (mPop2) {
        stack->popUint32();
    }
}

}