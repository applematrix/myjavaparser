#include "InvokeInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/Heap.h"
#include "../classloader/ClassFileInfo.h"
#include "../classloader/ConstantMethodRef.h"
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
    ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);

    uint16_t classIndex = methodRef->classIndex;
    // TODO: check the class is a inteface
    uint16_t nameAndTypeIndex = methodRef->nameAndTypeIndex;

    ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(nameAndTypeIndex);
    nameAndTypeRef->descriptorIndex;

    ConstantUtf8* method = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->nameIndex);
    const char* methodName = method->typeString();

    ConstantUtf8* methodDesc = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->descriptorIndex);
    
    cout << "InvokeSpecialInstruction, method:" << method->bytes
         << ", description:" << methodDesc->bytes << endl;

    // TODO:
}

/////////////////////////////////////////////////////////////////

InvokeVirtualInstruction::InvokeVirtualInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void InvokeVirtualInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {

    cout << "InvokeVirtualInstruction, index:" << (uint32_t)mIndex << endl;

    // ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);

    // uint16_t classIndex = methodRef->classIndex;
    // // TODO: check the class is a inteface
    // uint16_t nameAndTypeIndex = methodRef->nameAndTypeIndex;

    // ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(nameAndTypeIndex);
    // nameAndTypeRef->descriptorIndex;

    // ConstantUtf8* method = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->nameIndex);
    // const char* methodName = method->typeString();

    // ConstantUtf8* methodDesc = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->descriptorIndex);
    
    // cout << "InvokeSpecialInstruction, method:" << method->bytes
    //      << ", description:" << methodDesc->bytes << endl;

    // TODO:
}

}