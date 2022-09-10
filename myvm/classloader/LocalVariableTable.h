#ifndef _LOCAL_VARIABLE_TABLE_H_
#define _LOCAL_VARIABLE_TABLE_H_

#include "Method.h"

#include <stdint.h>

namespace myvm {

class LocalVariableTable {
public:
    LocalVariableTable(Method* method, uint16_t size);
    ~LocalVariableTable() {
        if (mLocalVariableArray != nullptr) {
            delete[] mLocalVariableArray;
        }
    }

    template<class T>
    void localVariableAt(uint16_t index, T& variable) {
        if (index + sizeof(T) > mLocalVariableSize) {
            // TODO: error!
        }
        variable = *(mLocalVariableArray + index);
    }

    void store(uint16_t index, uint32_t value);
    uint32_t variableAt(uint16_t index);
private:
    uint8_t *mLocalVariableArray;
    uint16_t mLocalVariableSize;
};

}
#endif