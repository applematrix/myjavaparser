#include "LoadInstructions.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/OperandStack.h"
#include "../classloader/CodeAttr.h"

#undef LOG_TAG
#define LOG_TAG "LoadInstruction"
#include "../common/Logger.h"

using namespace myvm;

namespace myvm {

void IloadInstruction::run(Frame* frame) {
    
}

void Iload0Instruction::run(Frame* frame) {
}

void Iload1Instruction::run(Frame* frame) {
}

////////////////////////////////////////////////////

AloadInstruction::AloadInstruction(uint8_t *code) {
    mOpCode = *code;
    mLocalVariableTableIndex = *(code+1);
}

AloadInstruction::AloadInstruction(uint8_t *code, uint8_t index) {
    mOpCode = *code;
    mLocalVariableTableIndex = index;
}

void AloadInstruction::run(Frame* frame) {
    // test code
    shared_ptr<Method> method = frame->getMethod();
    shared_ptr<CodeAttr> codeAttr = method->getCodeAttr();
    auto attr = codeAttr->getAttributeByType(ATTR_LOCAL_VARIABLE_TABLE);

    shared_ptr<LocalVariableTable> lvt = frame->getLocalVariableTable();
    if (lvt == nullptr) {
        LOGW("No local variable table");
        return;
    }
    uint32_t objectRef = lvt->variableAt(mLocalVariableTableIndex);

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushUint32(objectRef);
    
    LOGD("%sAloadInstruction run, load object at local variable[%d]=%d, into the stack"
            ", current stack size = %d", INDENTS[frame->getDepth()], (uint32_t)mLocalVariableTableIndex, objectRef, stack->getSize());
}


}
