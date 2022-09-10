#include "Frame.h"
#include "Method.h"
#include "LocalVariableTable.h"

using namespace myvm;

namespace myvm {

Frame::Frame(Method *method, uint32_t localVariableSize, uint16_t depth) {
    mLocalVariableTable = new LocalVariableTable(method, localVariableSize);
    mClazz = method->getClass();
    mMethod = method;
    mCallDepth = depth;
}

Frame::~Frame() {
}

}