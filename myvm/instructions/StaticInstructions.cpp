#include "StaticInstructions.h"
#include "../classloader/OperandStack.h"
#include "../classloader/LocalVariableTable.h"
#include "../classloader/Method.h"
#include "../classloader/Frame.h"
#include "../classloader/ConstantFieldRef.h"

#undef LOG_TAG
#define LOG_TAG "StaticInstruction"
#include "../common/Logger.h"

using namespace myvm;

namespace myvm {

GetStaticInstruction::GetStaticInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void GetStaticInstruction::run(Frame* frame) {
    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    ClassInfo* clazz = frame->getClass();
    auto constantInfo = clazz->getConstant<ConstantFieldRef>(mIndex);
    if (constantInfo->tag != ConstantTag::CONSTANT_FIELDREF) {
        LOGW("GetStaticInstruction run: invalid index reference a non-field");
        return;
    }
    auto fieldRef = dynamic_cast<ConstantFieldRef*>(constantInfo);

    auto fieldClass = clazz->getConstant<ConstantClass>(fieldRef->classIndex);
    auto fieldName = clazz->getConstant<ConstantNameAndType>(fieldRef->nameAndTypeIndex);

    //LOGW("GetStaticInstruction run: class:%s field:%s", fieldClass->bytes, fieldName->bytes);
}

}
