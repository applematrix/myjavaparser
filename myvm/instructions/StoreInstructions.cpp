#undef LOG_TAG
#define LOG_TAG "AStoreInstruction"
#include "../common/Logger.h"

#include "StoreInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"
#include "../classloader/Frame.h"

using namespace myvm;

namespace myvm {

AStoreInstruction::AStoreInstruction(uint8_t *code) {
    mOpCode = *code;
    mLocalVariableTableIndex = *(code+1);
}

AStoreInstruction::AStoreInstruction(uint8_t *code, uint8_t index) {
    mOpCode = *code;
    mLocalVariableTableIndex = index;
}

void AStoreInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    shared_ptr<LocalVariableTable> lvt = frame->getLocalVariableTable();

    if (lvt == nullptr) {
        LOGW("No local variable table");
        return;
    }
    uint32_t handle = stack->popUint32();

    LOGD("%sAStoreInstruction run: store handle = %d"
            " at the top of stack into localVariableTable[%d]"
            ", current stack size = %d", INDENTS[frame->getDepth()], handle, (uint32_t)mLocalVariableTableIndex, stack->getSize());
    // TODO: check the handle
    lvt->storeUint32(mLocalVariableTableIndex, handle);
}

}
