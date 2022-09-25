#include "NewInstruction.h"
#include "../classloader/ClassFileInfo.h"
#include "../classloader/OperandStack.h"
#include "../classloader/ObjectRef.h"
#include "../classloader/Heap.h"
#include "../classloader/BootstrapClassLoader.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

NewInstruction::NewInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void NewInstruction::run(ClassFileInfo* clazz, Method *context, OperandStack *stack) {
    // ConstantClass*constantInfo = (ConstantClass*)clazz->getConstantAt(mIndex);

    // const char* constantName = constantInfo->typeString();
    
    // ConstantUtf8* constantUtf8 = (ConstantUtf8*)clazz->getConstantAt(constantInfo->nameIndex);
    // cout << "new Instance, type:" << constantInfo->typeString()
    //     << ", binary name:" << constantUtf8->bytes << endl;

    // // TODO: release the memory
    // const char* className = (const char*)constantUtf8->bytes;
    // uint32_t handle = Heap::getInstance()->allocateObject(className);
    // stack->pushUint32(handle);
}

void NewInstruction::run(Frame *frame) {
    ClassFileInfo *clazz = frame->getClass();
    ConstantClass*constantInfo = (ConstantClass*)clazz->getConstantAt(mIndex);
    const char* constantName = constantInfo->typeString();
    
    ConstantUtf8* constantUtf8 = (ConstantUtf8*)clazz->getConstantAt(constantInfo->nameIndex);

    // TODO: release the memory
    const char* className = (const char*)constantUtf8->bytes;
    BootstrapClassLoader *bootstrapClassLoder = BootstrapClassLoader::getInstance();
    
    ClassFileInfo *instanceClazz = bootstrapClassLoder->getClassByName(className);
    uint32_t handle = Heap::getInstance()->allocateObject(instanceClazz);

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushUint32(handle);
    cout << INDENTS[frame->getDepth()] << "new Instance, type:" << constantInfo->typeString()
        << ", binary name:" << constantUtf8->bytes
        << ", push handle=" << handle << " into the stack"
        << ", current stack size =" << stack->getSize() << endl;
}

}