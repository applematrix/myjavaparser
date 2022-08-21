#include "LoadInstructions.h"

namespace myvm {

void IloadInstruction::run(Method *context) {
    
}

void Iload0Instruction::run(Method *context) {
}

void Iload1Instruction::run(Method *context) {
}

//
AloadInstruction::AloadInstruction(uint8_t *code) {
    index = *(code+1);
}

void AloadInstruction::run(Method *context) {
}

}