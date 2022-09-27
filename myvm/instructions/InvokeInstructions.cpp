#include "InvokeInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/Heap.h"
#include "../classloader/ClassFileInfo.h"
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
    ClassFileInfo *clazz = frame->getClass();

    ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);
    ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(methodRef->nameAndTypeIndex);
    Method* method = clazz->findMethod(nameAndTypeRef);

    if (method == nullptr) {
        cout << "InvokeSpecialInstruction error: no method found!" << endl;
    }

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

    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();

    Method* targetMethod = targetClazz->findMethod(targetMethodName, targetMethodDesc);
    vector<shared_ptr<TypeInfo>> args = targetMethod->getArgs();
    
    CodeAttr* codeAttr = targetMethod->getCodeAttr();
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

    cout << INDENTS[frame->getDepth()] << "InvokeSpecialInstruction, target class:" << targetClassName->bytes
         << ", target method:" << targetMethodName->bytes
         << ", description:" << targetMethodDesc->bytes 
         << ", pop the args from the stack, current stack size = " << curStack->getSize()<< endl;

    cout << INDENTS[frame->getDepth()] << "{" << endl;
    targetMethod->invoke(newFrame);
    cout << INDENTS[frame->getDepth()] << "}" << endl;
}

/////////////////////////////////////////////////////////////////

InvokeVirtualInstruction::InvokeVirtualInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void InvokeVirtualInstruction::run(Frame *frame) {
    ClassFileInfo *clazz = frame->getClass();
    ConstantMethodRef* methodRef = (ConstantMethodRef*)clazz->getConstantAt(mIndex);
    ConstantNameAndType* nameAndTypeRef = (ConstantNameAndType*)clazz->getConstantAt(methodRef->nameAndTypeIndex);
    Method* method = clazz->findMethod(nameAndTypeRef);

    if (method == nullptr) {
        cout << "InvokeVirtualInstruction error: no method found!" << endl;
    }

    shared_ptr<OperandStack> curStack = ThreadLocalStorage::getInstance()->getStack();
    vector<shared_ptr<TypeInfo>> args = method->getArgs();
    
    CodeAttr* codeAttr = method->getCodeAttr();
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

    cout << "InvokeVirtualInstruction, method:" << method->getName()
        << ", description:" << method->getDesc() 
        << ", pop the args from the stack, current stack size = " << curStack->getSize()<< endl;

    cout << INDENTS[frame->getDepth()] << "{" << endl;
    method->invoke(newFrame);
    cout << INDENTS[frame->getDepth()] << "}" << endl;
}

}