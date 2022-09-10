#include "FieldInstructions.h"
#include "../classloader/Method.h"
#include "../classloader/ClassFileInfo.h"
#include "../classloader/ConstantInfo.h"
#include "../classloader/ConstantFieldRef.h"
#include "../classloader/BootstrapClassLoader.h"

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
    ClassFileInfo *clazz = frame->getClass();
    ConstantFieldRef *fieldRef = (ConstantFieldRef *)clazz->getConstantAt(mIndex);

    ConstantClass *classInfo = (ConstantClass*)clazz->getConstantAt(fieldRef->classIndex);
    ConstantNameAndType *typeInfo = (ConstantNameAndType*)clazz->getConstantAt(fieldRef->nameAndTypeIndex);

    ConstantUtf8 *name = (ConstantUtf8*)clazz->getConstantAt(typeInfo->nameIndex);
    ConstantUtf8 *desc = (ConstantUtf8*)clazz->getConstantAt(typeInfo->descriptorIndex);

    cout << INDENTS[frame->getDepth()] << "put field, index:"
        << mIndex << ", name:" << name->bytes << ", description:" << desc->bytes << endl;
}

}