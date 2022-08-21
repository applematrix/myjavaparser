#include "LoadInstructions.h"

namespace myvm {

void IloadInstruction::run(ClassFileInfo* clazz, Method *context) {
    
}

void Iload0Instruction::run(ClassFileInfo* clazz, Method *context) {
}

void Iload1Instruction::run(ClassFileInfo* clazz, Method *context) {
}

//
AloadInstruction::AloadInstruction(uint8_t *code) {
    index = *(code+1);
}

void AloadInstruction::run(ClassFileInfo* clazz, Method *context) {
}

}