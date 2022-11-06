#undef LOG_TAG
#define LOG_TAG "FieldInstruction"
#include "common/Logger.h"

#include "FieldInstructions.h"
#include "../classloader/Method.h"
#include "../classloader/ClassInfo.h"
#include "../classloader/ConstantInfo.h"
#include "../classloader/ConstantFieldRef.h"
#include "../classloader/BootstrapClassLoader.h"
#include "../classloader/Heap.h"
#include "../classloader/Object.h"

#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

GetFieldInstruction::GetFieldInstruction(uint8_t *code) {
    uint8_t index1 = *(code+1);
    uint8_t index2 = *(code+2);
    mIndex = (index1 << 8) | index2;
}

void GetFieldInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();
    shared_ptr<ConstantFieldRef> fieldRef = dynamic_pointer_cast<ConstantFieldRef>(clazz->getConstantAt(mIndex));

    shared_ptr<ConstantClass> classInfo = dynamic_pointer_cast<ConstantClass>(clazz->getConstantAt(fieldRef->classIndex));
    shared_ptr<ConstantNameAndType> typeInfo = dynamic_pointer_cast<ConstantNameAndType>(clazz->getConstantAt(fieldRef->nameAndTypeIndex));

    shared_ptr<ConstantUtf8> name = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(typeInfo->nameIndex));
    shared_ptr<ConstantUtf8> desc = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(typeInfo->descriptorIndex));

    shared_ptr<FieldInfo> field = clazz->findField(typeInfo->nameIndex, typeInfo->descriptorIndex);
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    uint32_t handle = stack->popUint32();
    auto object = Heap::getInstance()->getObject(handle);

    if (field->getType()->doubleUnit()) {
        uint64_t value = object->getUint64Field(field->offsetInClass());
        stack->pushUint64(value);

        LOGD("%sGetFieldInstruction run, index:= %d, name:%s, description:%s"
                ", push long value = %ld into the stack"
                ", current stack size = %d", INDENTS[frame->getDepth()], mIndex, name->bytes, desc->bytes, value, stack->getSize());
    } else {
        uint32_t value = object->getField(field->offsetInClass());
        stack->pushUint32(value);

        LOGD("%sGetFieldInstruction run, index:= %d, name:%s, description:%s"
                ", push long value = %d into the stack"
                ", current stack size = %d", INDENTS[frame->getDepth()], mIndex, name->bytes, desc->bytes, value, stack->getSize());
    }
}


/////////////////////////////////////////////////////////
PutFieldInstruction::PutFieldInstruction(uint8_t *code) {
    uint8_t index1 = *(code+1);
    uint8_t index2 = *(code+2);
    mIndex = (index1 << 8) | index2;
}

void PutFieldInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();
    shared_ptr<ConstantFieldRef> fieldRef = dynamic_pointer_cast<ConstantFieldRef>(clazz->getConstantAt(mIndex));

    shared_ptr<ConstantClass> classInfo = dynamic_pointer_cast<ConstantClass>(clazz->getConstantAt(fieldRef->classIndex));
    shared_ptr<ConstantNameAndType> typeInfo = dynamic_pointer_cast<ConstantNameAndType>(clazz->getConstantAt(fieldRef->nameAndTypeIndex));

    shared_ptr<ConstantUtf8> name = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(typeInfo->nameIndex));
    shared_ptr<ConstantUtf8> desc = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(typeInfo->descriptorIndex));
   
    LOGD("%sPutFieldInstruction run, index:= %d, name:%s, description:%s", INDENTS[frame->getDepth()], mIndex, name->bytes, desc->bytes);
    
    shared_ptr<FieldInfo> field = clazz->findField(typeInfo->nameIndex, typeInfo->descriptorIndex);
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    if (field->getType()->doubleUnit()) {
        uint64_t value = stack->popUint64();
        uint32_t handle = stack->popUint32();
        auto object = Heap::getInstance()->getObject(handle);
        object->putField(field->offsetInClass(), value);
        
        LOGD("%sPutFieldInstruction run, index:= %d, name:%s, description:%s"
                ", pop value = %ld from the stack into the field, "
                ", current stack size = %d", INDENTS[frame->getDepth()], mIndex, name->bytes, desc->bytes, value, stack->getSize());

    } else {
        uint32_t value = stack->popUint32();
        uint32_t handle = stack->popUint32();
        auto object = Heap::getInstance()->getObject(handle);
        object->putField(field->offsetInClass(), value);

        LOGD("%sPutFieldInstruction run, index:= %d, name:%s, description:%s"
                ", pop value = %d from the stack into the field, "
                ", current stack size = %d", INDENTS[frame->getDepth()], mIndex, name->bytes, desc->bytes, value, stack->getSize());
    }
}

}
