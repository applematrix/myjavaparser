#include "Heap.h"
#include "ObjectRef.h"

namespace myvm {

Heap* Heap::sInstance = nullptr;

Heap* Heap::getInstance() {
    // TODO: multi thread
    if (sInstance == nullptr) {
        sInstance = new Heap();
    }
    return sInstance;
}

ObjectRef* Heap::getReference(uint32_t handle) {
    if (mObjectRefs.find(handle) != mObjectRefs.end()) {
        return mObjectRefs[handle];
    }
    return nullptr;
}


uint32_t Heap::allocateObject(const char* name) {
    ObjectRef *object = new ObjectRef(name);
    mObjectRefs[mNextHandle] = object;
    // TODO: multi thread
    mNextHandle++;
    return mNextHandle-1;
}

}