#undef LOG_TAG
#define LOG_TAG "InvokeSpecialInstruction"
#include "../common/Logger.h"

#include "InvokeInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/Heap.h"
#include "../classloader/ClassInfo.h"
#include "../classloader/ConstantMethodRef.h"
#include "../classloader/BootstrapClassLoader.h"
#include "../classloader/CodeAttr.h"
#include "../classloader/Frame.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace myvm;

namespace myvm {

InvokeSpecialInstruction::InvokeSpecialInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void InvokeSpecialInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();

    shared_ptr<ConstantMethodRef> methodRef = dynamic_pointer_cast<ConstantMethodRef>(clazz->getConstantAt(mIndex));
    shared_ptr<ConstantNameAndType> nameAndTypeRef = dynamic_pointer_cast<ConstantNameAndType>(clazz->getConstantAt(methodRef->nameAndTypeIndex));
    shared_ptr<Method> method = clazz->findMethod(nameAndTypeRef);

    if (method == nullptr) {
        LOGW("InvokeSpecialInstruction error: no method found!");
    }

    shared_ptr<ConstantClass> targetClassInfo = dynamic_pointer_cast<ConstantClass>(clazz->getConstantAt(methodRef->classIndex));
    shared_ptr<ConstantUtf8> targetClassName = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(targetClassInfo->nameIndex));

    // TODO: must use the class' loader to load the class
    shared_ptr<ClassInfo> targetClazz = BootstrapClassLoader::getInstance()->getClassByName(string((const char*)targetClassName->bytes));
    if (targetClazz == nullptr) {
        LOGW("Class not load, we must load it first!");
        // TODO:
    }
    
    shared_ptr<ConstantUtf8> targetMethodName = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(nameAndTypeRef->nameIndex));
    shared_ptr<ConstantUtf8> targetMethodDesc = dynamic_pointer_cast<ConstantUtf8>(clazz->getConstantAt(nameAndTypeRef->descriptorIndex));

    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();

    shared_ptr<Method> targetMethod = targetClazz->findMethod(targetMethodName, targetMethodDesc);
    vector<shared_ptr<TypeInfo>> args = targetMethod->getArgs();
    
    shared_ptr<CodeAttr> codeAttr = targetMethod->getCodeAttr();
    shared_ptr<Frame> newFrame = make_shared<Frame>(targetMethod, codeAttr->maxLocals, frame->getDepth() + 1);
    auto attr = codeAttr->getAttributeByType(ATTR_LOCAL_VARIABLE_TABLE);
    if (attr == nullptr) {
        // Error?!
    }

    shared_ptr<LocalVariableTable> localVariableTable = newFrame->getLocalVariableTable();
    shared_ptr<LocalVariableTableAttr> lvtAttr = dynamic_pointer_cast<LocalVariableTableAttr>(attr);
    for (int i = args.size() - 1; i > 0; i--) {
        shared_ptr<TypeInfo> type = args.at(i);
        uint16_t index = lvtAttr->localVariables.at(i)->index;
        if(type->doubleUnit()) {
            uint64_t operand = curStack->popUint64();
            localVariableTable->storeUint64(index, operand);
        } else {
            uint32_t operand = curStack->popUint32();
            localVariableTable->storeUint32(index, operand);
        }
    }
    if (!targetMethod->isStatic()) {
        uint32_t objectRef = curStack->popUint32();
        localVariableTable->storeUint32(0, objectRef);
    }

    LOGD("%sInvokeSpecialInstruction, target class:%s, target method:%s, description:%s" 
            ", pop the args from the stack, current stack size = %d", 
            INDENTS[frame->getDepth()], targetClassName->bytes, targetMethodName->bytes, targetMethodDesc->bytes, curStack->getSize());

    LOGD("%s{", INDENTS[frame->getDepth()]);
    targetMethod->invoke(newFrame);
    LOGD("%s}", INDENTS[frame->getDepth()]);
}

/////////////////////////////////////////////////////////////////

InvokeVirtualInstruction::InvokeVirtualInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void InvokeVirtualInstruction::run(Frame *frame) {
    ClassInfo *clazz = frame->getClass();
    shared_ptr<ConstantMethodRef> methodRef = dynamic_pointer_cast<ConstantMethodRef>(clazz->getConstantAt(mIndex));
    shared_ptr<ConstantNameAndType> nameAndTypeRef = dynamic_pointer_cast<ConstantNameAndType>(clazz->getConstantAt(methodRef->nameAndTypeIndex));
    shared_ptr<Method> method = clazz->findMethod(nameAndTypeRef);

    if (method == nullptr) {
        LOGW("InvokeVirtualInstruction error: no method found!");
    }

    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();
    vector<shared_ptr<TypeInfo>> args = method->getArgs();
    
    shared_ptr<CodeAttr> codeAttr = method->getCodeAttr();
    shared_ptr<Frame> newFrame = make_shared<Frame>(method, codeAttr->maxLocals, frame->getDepth() + 1);
    auto attr = codeAttr->getAttributeByType(ATTR_LOCAL_VARIABLE_TABLE);
    if (attr == nullptr) {
        // Error?!
    }

    shared_ptr<LocalVariableTable> localVariableTable = newFrame->getLocalVariableTable();
    shared_ptr<LocalVariableTableAttr> lvtAttr = dynamic_pointer_cast<LocalVariableTableAttr>(attr);
    for (int i = args.size() - 1; i > 0; i--) {
        shared_ptr<TypeInfo> type = args.at(i);
        uint16_t index = lvtAttr->localVariables.at(i)->index;
        if(type->doubleUnit()) {
            uint64_t operand = curStack->popUint64();
            localVariableTable->storeUint64(index, operand);
        } else {
            uint32_t operand = curStack->popUint32();
            localVariableTable->storeUint32(index, operand);
        }
    }

    uint32_t objectRef = curStack->popUint32();
    localVariableTable->storeUint32(0, objectRef);

    LOGD("%sInvokeVirtualInstruction, method:%s, description:%s" 
            ", pop the args from the stack, current stack size = %d", INDENTS[frame->getDepth()], method->getName(), method->getDesc(), curStack->getSize());


    LOGD("%s{", INDENTS[frame->getDepth()]);
    method->invoke(newFrame);
    LOGD("%s}", INDENTS[frame->getDepth()]);
}

}
