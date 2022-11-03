#include "Object.h"

namespace myvm {

Object::Object(shared_ptr<ClassInfo>& clazzInfo) {
    mClazz = clazzInfo;
    mFields = new uint32_t[clazzInfo->classSize()];
    if (!clazzInfo->getSuperClass().expired()) {
        mFieldOffset = clazzInfo->getSuperClass().lock()->classSize();
    } else {
        mFieldOffset = 0;
    }
}

Object::~Object() {
    delete []mFields;
    mFields = nullptr;
}

void Object::putField(uint32_t index, uint32_t value) {
    mFields[mFieldOffset + index] = value;
}
    
void Object::putField(uint32_t index, uint64_t value) {
    mFields[mFieldOffset + index] = value;
}

uint32_t Object::getField(uint32_t index) {
    return mFields[mFieldOffset + index];
}

uint64_t Object::getUint64Field(uint32_t index) {
    return mFields[mFieldOffset + index];
}

}