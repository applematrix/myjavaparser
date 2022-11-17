#include "NewInstruction.h"
#include "../classloader/ClassInfo.h"
#include "../classloader/OperandStack.h"
#include "../classloader/ObjectRef.h"
#include "../classloader/Heap.h"
#include "../classloader/BootstrapClassLoader.h"
#include <iostream>

#undef LOG_TAG
#define LOG_TAG "NewInstruction"
#include "../common/Logger.h"

using namespace std;
using namespace myvm;

namespace myvm {

NewInstruction::NewInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void NewInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();
    auto constantInfo = clazz->getConstant<ConstantClass>(mIndex);
    const char* constantName = constantInfo->typeString();
    
    auto constantUtf8 = clazz->getConstant<ConstantUtf8>(constantInfo->nameIndex);

    // TODO: release the memory
    const char* className = (const char*)constantUtf8->bytes;
    auto classLoader = clazz->getClassLoader().lock();
    if (classLoader == nullptr) {
        return;
    }
    
    shared_ptr<ClassInfo> instanceClazz = classLoader->getClassByName(className);
    uint32_t handle = Heap::getInstance()->allocateObject(instanceClazz);

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushUint32(handle);
    
    LOGD("%sNewInstruction run, new Instance, type:%s"
            ", binary name:%s"
            ", push handle=%d into the stack, current stack size = %d", 
            INDENTS[frame->getDepth()], constantInfo->typeString(), constantUtf8->bytes, handle, stack->getSize());
}

}
