#include "GetFieldInstruction.h"
#include "../classloader/Method.h"

namespace myvm {

GetFieldInstruction::GetFieldInstruction(uint8_t *code) {
    uint8_t index1 = *(code+1);
    uint8_t index2 = *(code+2);
    index = (index1 << 8) | index2;
}

void GetFieldInstruction::run(Method *context) {
}

}