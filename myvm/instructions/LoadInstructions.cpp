#include "LoadInstructions.h"

namespace myvm {

void IloadInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    
}

void Iload0Instruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
}

void Iload1Instruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
}

//
AloadInstruction::AloadInstruction(uint8_t *code) {
    index = *(code+1);
}

void AloadInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
}

}