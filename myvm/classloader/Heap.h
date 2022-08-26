#ifndef _HEAP_H_
#define _HEAP_H_

#include <map>
using namespace std;

namespace myvm {
class ObjectRef;

class Heap {
public:
    static Heap* getInstance();

    ~Heap() {}
    ObjectRef* getReference(uint32_t handle);
    uint32_t allocateObject(const char* name);
private:
    Heap() {
        mNextHandle = 0;
    }
    static Heap* sInstance;
private:
    map<uint32_t, ObjectRef*> mObjectRefs;
    uint32_t mNextHandle;
};

}

#endif