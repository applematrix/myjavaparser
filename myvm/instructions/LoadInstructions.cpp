#include "LoadInstructions.h"
#include "classloader/LocalVariableTable.h"
#include "classloader/OperandStack.h"
#include "classloader/CodeAttr.h"

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
    Method *method = frame->getMethod();
    CodeAttr* codeAttr = method->getCodeAttr();
    auto attr = codeAttr->getAttributeByType(ATTR_LOCAL_VARIABLE_TABLE);

    shared_ptr<LocalVariableTable> lvt = frame->getLocalVariableTable();
    if (lvt == nullptr) {
        cout << "No local variable table" << endl;
        return;
    }
    uint32_t objectRef = lvt->variableAt(mLocalVariableTableIndex);

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushUint32(objectRef);
    cout << INDENTS[frame->getDepth()] << "AloadInstruction run: load object at local variable[" << (uint32_t)mLocalVariableTableIndex << "]="
        << objectRef <<" into the stack"
        << ", current stack size =" << stack->getSize()<< endl;
}


}