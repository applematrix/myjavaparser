#include "StoreInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"

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
    ObjectRef* objectRef = stack->popObjecRef();
    cout << "AStoreInstruction run: store " << (uint32_t)mLocalVariableTableIndex << " into the stack! " << endl;

    LocalVariableTable* lvt = context->getLocalVariableTable();
    if (lvt == nullptr) {
        cout << "No local variable table" << endl;
        return;
    }
    lvt->store(mLocalVariableTableIndex, (uint32_t)objectRef);
}

}