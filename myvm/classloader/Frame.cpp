#include "Frame.h"

namespace myvm {

Frame::Frame(ClassFileInfo *clazz, Method *method, uint32_t stackSize, uint32_t localVariableSize) {
    mStack = new OperandStack(stackSize);
    mLocalVariableTable = new LocalVariableTable(localVariableSize);
    mClazz = clazz;
    mMethod = method;
}

Frame::~Frame() {
}

}