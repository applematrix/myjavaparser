/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "Method.h"
#include "ClassFileInfo.h"
#include "FileReader.h"
#include "AttributeInfo.h"
#include "AccessFlag.h"
#include "OperandStack.h"
#include "LocalVariableTable.h"
#include "Instructions.h"
#include "DupInstruction.h"
#include "ConstantInfo.h"
#include "Frame.h"

#include <iostream>

using namespace std;

namespace myvm {

Method::Method(uint16_t flags,
    uint16_t name,
    uint16_t desc,
    vector<AttributeInfo*> *attrs) {
    accessFlags = flags;
    nameIndex = name;
    descriptorIndex = desc;
    attributeCount = attrs->size();
    mAttributes = *attrs;

    mMainMethod = false;
}

Method::~Method() {
    mAttributes.clear();
}

void Method::invoke(ClassFileInfo *clazz) {
    if (isAbstract()) {
        cout << "Can't invoe abstract method!"<< endl;
        return;
    }
    if (mCodeAttr == nullptr) {
        cout << "No code for this method!" << endl;
        return;
    }

    OperandStack* stack = new OperandStack(mCodeAttr->maxStack);
    LocalVariableTable * localVariableTable = new LocalVariableTable(mCodeAttr->maxLocals);

    Frame * frame = new Frame(mCodeAttr->maxStack, mCodeAttr->maxLocals);

    // interprete the code
    uint8_t *code = mCodeAttr->code;
    uint8_t *codeEnd = code + mCodeAttr->codeLength;
    while (code < codeEnd) {
        auto instruction = Instruction::interpreteCode(code);
        if (instruction == nullptr) {
            cout << "Invalid instruction!" << endl;
        }
        instruction->run(clazz, this, stack);
        code += instruction->codeLen();
    }
    
    // release
    delete frame;
    delete stack;
    delete localVariableTable;
}

bool Method::isAbstract() {
    return (accessFlags & METHOD_ACC_ABSTRACT) != 0;
}

bool Method::isStatic() {
    return (accessFlags & METHOD_ACC_STATIC) != 0;
}

bool Method::isPublic() {
    return (accessFlags & METHOD_ACC_PUBLIC) != 0;
}

bool Method::isPrivate() {
    return (accessFlags & METHOD_ACC_PRIVATE) != 0;
}

bool Method::isProtected() {
    return (accessFlags & METHOD_ACC_PROTECTED) != 0;
}

Method* Method::loadFromFile(ClassFileInfo *classFileInfo, FileReader *fileReader) {
    uint16_t accessFlags = 0;
    int status = fileReader->readUint16(accessFlags);
    if (status != 0) {
        return nullptr;
    }

    uint16_t nameIndex = 0;
    status = fileReader->readUint16(nameIndex);
    if (status != 0) {
        return nullptr;
    }
    classFileInfo->printConstantInfo(nameIndex);

    uint16_t descriptorIndex = 0;
    status = fileReader->readUint16(descriptorIndex);
    if (status != 0) {
        return nullptr;
    }

    uint16_t attributeCount = 0;
    status = fileReader->readUint16(attributeCount);
    if (status != 0) {
        return nullptr;
    }

    cout << "Have "<< attributeCount <<" attributes!" << endl;
    auto attributes = new vector<AttributeInfo *>();
    if (attributeCount > 0) {
        for (int i = 0; i < attributeCount; i++) {
            cout << "Load attribute #" << i << endl;
            AttributeInfo *attrInfo = AttributeFactory::loadFromFile(classFileInfo, fileReader);
            if (attrInfo == nullptr) {
                cout << "Load attribute #" << i << " failed!" << endl;
                return nullptr;
            }
            attributes->push_back(attrInfo);
            cout << "Load attribute #" << i << " complete!" << endl;
        }
    }
    return new Method(accessFlags, nameIndex, descriptorIndex, attributes);
}

bool Method::match(const ConstantNameAndType* nameAndType) {
    // TODO: check the same class
    return nameIndex == nameAndType->nameIndex
        && descriptorIndex == nameAndType->descriptorIndex;
}

bool Method::isMainEntry() {
    return mMainMethod;
}

void Method::resolve(ClassFileInfo *clazz) {
    const char* name = clazz->getUtf8ConstantName(nameIndex);
    mName = std::move(std::string(name));

    const char* desc = clazz->getUtf8ConstantName(descriptorIndex);
    mDescriptor = std::move(std::string(desc));

    if (strncmp(name, "main", 4) == 0) {
        const char* mainMethodArgs = "([Ljava/lang/String;)V";
        mMainMethod = isPublic() && isStatic()
            && strncmp(desc, mainMethodArgs, strlen(mainMethodArgs)) == 0;
    }
    else if (strncmp(name, "<init>", 6) == 0) {
        mConstructor = true;
    }

    CodeAttr *codeInfo = nullptr;
    for (auto attr : mAttributes) {
        if (attr->attrType == ATTR_CODE) {
            mCodeAttr = (CodeAttr* )attr;
        }
    }

    if (desc == nullptr) {
        return;
    }
}

LocalVariableTable* Method::getLocalVariableTable() {
    // TODO:
    return nullptr;
}

OperandStack* Method::getOperandStack() {
    // TODO:
    return nullptr;
}

}