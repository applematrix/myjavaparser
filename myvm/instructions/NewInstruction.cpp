#undef LOG_TAG
#define LOG_TAG "NewInstruction"
#include "common/Logger.h"

#include "NewInstruction.h"
#include "../classloader/ClassInfo.h"
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

void NewInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();
    shared_ptr<ConstantClass> constantInfo = dynamic_pointer_cast<ConstantClass>(clazz->getConstantAt(mIndex));
    const char* constantName = constantInfo->typeString();
    
    shared_ptr<ConstantUtf8> constantUtf8 = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(constantInfo->nameIndex));

    // TODO: release the memory
    const char* className = (const char*)constantUtf8->bytes;
    BootstrapClassLoader *bootstrapClassLoder = BootstrapClassLoader::getInstance();
    
    shared_ptr<ClassInfo> instanceClazz = bootstrapClassLoder->getClassByName(className);
    uint32_t handle = Heap::getInstance()->allocateObject(instanceClazz);

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->pushUint32(handle);
    
    LOGD("%sNewInstruction run, new Instance, type:%s"
            ", binary name:%s"
            ", push handle=%d into the stack, current stack size = %d", 
            INDENTS[frame->getDepth()], constantInfo->typeString(), constantUtf8->bytes, handle, stack->getSize());
}

}
