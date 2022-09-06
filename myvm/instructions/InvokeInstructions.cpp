#include "InvokeInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/Heap.h"
#include "../classloader/ClassFileInfo.h"
#include "../classloader/ConstantMethodRef.h"
#include "../classloader/BootstrapClassLoader.h"
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
    ConstantClass* constantClass = (ConstantClass*)clazz->getConstantAt(classIndex);

    // TODO: find the correct class
    // TODO: check the class is a inteface
    uint16_t nameAndTypeIndex = methodRef->nameAndTypeIndex;
    ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(nameAndTypeIndex);
    Method* method = clazz->findMethod(nameAndTypeRef);

    
    if (method == nullptr) {
        cout << "InvokeSpecialInstruction error: no method found!" << endl;
    }

    // test code
    ConstantUtf8* methodUtf8 = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->nameIndex);
    const char* methodName = methodUtf8->typeString();
    ConstantUtf8* methodDesc = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->descriptorIndex);
    cout << "InvokeSpecialInstruction, method:" << methodUtf8->bytes
         << ", description:" << methodDesc->bytes << endl;

    method->invoke(clazz);
}

void InvokeSpecialInstruction::run(Frame *frame) {
    ClassFileInfo *clazz = frame->getClass();

    ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);
    ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(methodRef->nameAndTypeIndex);
    Method* method = clazz->findMethod(nameAndTypeRef);

    
    if (method == nullptr) {
        cout << "InvokeSpecialInstruction error: no method found!" << endl;
    }

    // test code
    ConstantClass* targetClassInfo = (ConstantClass*)clazz->getConstantAt(methodRef->classIndex);
    ConstantUtf8* targetClassName = (ConstantUtf8*)clazz->getConstantAt(targetClassInfo->nameIndex);

    // TODO: must use the class' loader to load the class
    ClassFileInfo* targetClazz = BootstrapClassLoader::getInstance()->getClassByName(string((const char*)targetClassName->bytes));
    if (targetClazz == nullptr) {
        cout << "Class not load, we must load it first!" << endl;
        // TODO:
    }
    
    ConstantUtf8* targetMethodName = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->nameIndex);
    ConstantUtf8* targetMethodDesc = (ConstantUtf8*)clazz->getConstantAt(nameAndTypeRef->descriptorIndex);
    cout << "InvokeSpecialInstruction, target class:" << targetClassName->bytes
         << ", target method:" << targetMethodName->bytes
         << ", description:" << targetMethodDesc->bytes << endl;

    Method* targetMethod = targetClazz->findMethod(targetMethodName, targetMethodDesc);
    targetMethod->invoke(targetClazz);
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

void InvokeVirtualInstruction::run(Frame *frame) {
    ClassFileInfo *clazz = frame->getClass();
    ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);
}

}