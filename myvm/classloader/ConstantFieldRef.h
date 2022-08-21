#ifndef _CONSTANT_FIELD_REF_H_
#define _CONSTANT_FIELD_REF_H_

#include "ConstantInfo.h"

namespace myvm {

class ConstantFieldRef: public ConstantRef {
public:
    ConstantFieldRef(uint8_t tag, uint16_t index, uint16_t nameAndType) : 
        ConstantRef(tag, index, nameAndType) {}
};

}

#endif