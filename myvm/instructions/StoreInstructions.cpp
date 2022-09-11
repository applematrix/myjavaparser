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

void AStoreInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    uint32_t handle = stack->popUint32();
    cout << "AStoreInstruction run: store " << (uint32_t)mLocalVariableTableIndex << " into the stack! " << endl;

    LocalVariableTable* lvt = context->getLocalVariableTable();
    if (lvt == nullptr) {
        cout << "No local variable table" << endl;
        return;
    }
    lvt->storeUint32(mLocalVariableTableIndex, handle);
}

void AStoreInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    shared_ptr<LocalVariableTable> lvt = frame->getLocalVariableTable();
}

}