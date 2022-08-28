#include "Frame.h"

namespace myvm {

Frame::Frame(uint32_t stackSize, uint32_t localVariableSize) {
    mStack = new OperandStack(stackSize);
    mLocalVariableTable = new LocalVariableTable(localVariableSize);
}

Frame::~Frame() {
}

}