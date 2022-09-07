#include "Frame.h"

namespace myvm {

Frame::Frame(ClassFileInfo *clazz, Method *method, uint32_t stackSize, uint32_t localVariableSize, uint16_t depth) {
    mStack = new OperandStack(stackSize);
    mLocalVariableTable = new LocalVariableTable(localVariableSize);
    mClazz = clazz;
    mMethod = method;
    mCallDepth = depth;
}

Frame::~Frame() {
}

}