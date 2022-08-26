#include "InvokeInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/Heap.h"
#include "../classloader/ClassFileInfo.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

InvokeSpecialInstruction::InvokeSpecialInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void InvokeSpecialInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    ConstantClass*constantInfo = (ConstantClass*)clazz->getConstantAt(mIndex);

    const char* constantName = constantInfo->typeString();
    
    ConstantUtf8* constantUtf8 = (ConstantUtf8*)clazz->getConstantAt(constantInfo->nameIndex);
    cout << "new Instance, type:" << constantInfo->typeString()
        << ", binary name:" << constantUtf8->bytes << endl;

    // TODO: release the memory
    const char* className = (const char*)constantUtf8->bytes;
    uint32_t handle = Heap::getInstance()->allocateObject(className);
    stack->pushUint32(handle);
}

}