#undef LOG_TAG
#define LOG_TAG "DupInstruction"
#include "common/Logger.h"

#include "DupInstruction.h"
#include "../classloader/OperandStack.h"
#include "../classloader/ThreadLocalStorage.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

void DupInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    uint32_t handle = stack->popUint32();
    
    stack->pushUint32(handle);
    stack->pushUint32(handle);
    LOGD("%DupInstruction run, Duplicate stack top instance= %d"
        ", current stack size = %d", INDENTS[frame->getDepth()], handle, stack->getSize());
}

}
