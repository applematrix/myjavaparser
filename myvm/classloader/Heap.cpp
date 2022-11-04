/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "Heap.h"
#include "ObjectRef.h"
#include "Object.h"
#include "classloader/ClassInfo.h"

using namespace myvm;

namespace myvm {

Heap* Heap::sInstance = nullptr;

Heap* Heap::getInstance() {
    // TODO: multi thread
    if (sInstance == nullptr) {
        sInstance = new Heap();
    }
    return sInstance;
}

shared_ptr<Object> Heap::getObject(uint32_t handle) {
    if (mObjectRefs.find(handle) != mObjectRefs.end()) {
        return mObjectRefs[handle];
    }
    return nullptr;
}

uint32_t Heap::allocateObject(shared_ptr<ClassInfo> &clazz) {
    uint32_t class_size = clazz->classSize();
    shared_ptr<Object> object = make_shared<Object>(clazz);
    mObjectRefs[mNextHandle] = object;
    // TODO: multi thread
    mNextHandle++;
    if (mNextHandle == -1) {
        mNextHandle = FIRST_HANDLE;
    }
    return mNextHandle-1;
}

}