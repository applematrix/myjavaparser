#include "FieldInstructions.h"
#include "../classloader/Method.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

GetFieldInstruction::GetFieldInstruction(uint8_t *code) {
    uint8_t index1 = *(code+1);
    uint8_t index2 = *(code+2);
    mIndex = (index1 << 8) | index2;
}

void GetFieldInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "get field" << endl;
}

void GetFieldInstruction::run(Frame *frame) {
    cout << "get field" << endl;
}


/////////////////////////////////////////////////////////
PutFieldInstruction::PutFieldInstruction(uint8_t *code) {
    uint8_t index1 = *(code+1);
    uint8_t index2 = *(code+2);
    mIndex = (index1 << 8) | index2;
}

void PutFieldInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    cout << "put field" << endl;
}

void PutFieldInstruction::run(Frame *frame) {
    cout << INDENTS[frame->getDepth()] << "put field" << endl;
}

}