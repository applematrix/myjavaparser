/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _HEAP_H_
#define _HEAP_H_

#define FIRST_HANDLE 0x10

#include <map>
#include <memory>
using namespace std;

namespace myvm {
class ObjectRef;
class Object;
class ClassInfo;

class Heap {
public:
    static Heap* getInstance();

    ~Heap() {}
    Object* getObject(uint32_t handle);
    uint32_t allocateObject(shared_ptr<ClassInfo> &name);
private:
    Heap() {
        mNextHandle = FIRST_HANDLE;
    }
    static Heap* sInstance;
private:
    map<uint32_t, Object*> mObjectRefs;
    uint32_t mNextHandle;
};

}

#endif