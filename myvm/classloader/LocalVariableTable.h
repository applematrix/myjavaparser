#ifndef _LOCAL_VARIABLE_TABLE_H_
#define _LOCAL_VARIABLE_TABLE_H_

#include <stdint.h>

namespace myvm {

class LocalVariableTable {
public:
    LocalVariableTable(uint16_t size);
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
private:
    uint8_t *mLocalVariableArray;
    uint16_t mLocalVariableSize;
};

}
#endif