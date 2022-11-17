#include "PopInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"

#undef LOG_TAG
#define LOG_TAG "PopInstruction"
#include "../common/Logger.h"

using namespace myvm;

namespace myvm {

void PopInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    // TODO:
    stack->popUint32();

    LOGD("%sPopInstruction run: pop %d  items from the stack"
            ", current stack size =%d", INDENTS[frame->getDepth()], (mPop2 ? 2 : 1), stack->getSize());
    if (mPop2) {
        stack->popUint32();
    }
}

}
