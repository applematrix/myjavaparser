#include "Frame.h"
#include "Method.h"
#include "LocalVariableTable.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

Frame::Frame(Method *method, uint32_t localVariableSize, uint16_t depth) {
    mLocalVariableTable = make_shared<LocalVariableTable>(method, localVariableSize);
    mClazz = method->getClass();
    mMethod = method;
    mCallDepth = depth;
}

Frame::~Frame() {
}

}