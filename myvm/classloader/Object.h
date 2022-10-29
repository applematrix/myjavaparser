#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ClassInfo.h"

using namespace myvm;

namespace myvm {

class Object {
public:
    Object(const ClassInfo* clazzInfo);
    virtual ~Object();
    void putField(uint32_t index, uint32_t value);
    void putField(uint32_t index, uint64_t value);
    uint32_t getField(uint32_t index);
    uint64_t getUint64Field(uint32_t index);
private:
    uint32_t *mFields;
    const ClassInfo *mClazz;
    uint32_t mFieldOffset;
};

}
#endif