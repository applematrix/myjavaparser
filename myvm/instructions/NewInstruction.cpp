#include "NewInstruction.h"
#include "../classloader/ClassFileInfo.h"
#include <iostream>

using namespace std;
using namespace myvm;

namespace myvm {

NewInstruction::NewInstruction(uint8_t *code) {
    uint8_t highByte = *(code+1);
    uint8_t lowByte = *(code+2);

    mIndex = (highByte<<8) | lowByte;
}

void NewInstruction::run(ClassFileInfo* clazz, Method *context) {
    ConstantClass*constantInfo = (ConstantClass*)clazz->getConstantAt(mIndex);

    const char* constantName = constantInfo->typeString();
    
    ConstantUtf8* constantUtf8 = (ConstantUtf8*)clazz->getConstantAt(constantInfo->nameIndex);
    cout << "new Instance, type:" << constantInfo->typeString()
        << ", binary name:" << constantUtf8->bytes << endl;

}

}