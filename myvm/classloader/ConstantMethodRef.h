#ifndef _CONSTANT_METHOD_REF_H_
#define _CONSTANT_METHOD_REF_H_

#include "ConstantInfo.h"

namespace myvm{

class ConstantMethodRef: public ConstantRef {
public:
    ConstantMethodRef(uint8_t tag, uint16_t index, uint16_t nameAndType) : 
        ConstantRef(tag, index, nameAndType) {}
};

}

#endif