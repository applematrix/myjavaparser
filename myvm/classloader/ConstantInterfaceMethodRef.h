#ifndef _CONSTANT_INTERFACE_METHOD_REF_H_
#define _CONSTANT_INTERFACE_METHOD_REF_H_

#include "ConstantInfo.h"

namespace myvm{

class ConstantInterfaceMethodRef: public ConstantRef {
public:
    ConstantInterfaceMethodRef(uint8_t tag, uint16_t index, uint16_t nameAndType) : 
        ConstantRef(tag, index, nameAndType) {}
};

}

#endif