/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "Method.h"
#include "ClassInfo.h"
#include "FileReader.h"
#include "AttributeInfo.h"
#include "AccessFlag.h"
#include "OperandStack.h"
#include "LocalVariableTable.h"
#include "Instructions.h"
#include "DupInstruction.h"
#include "ConstantInfo.h"
#include "Frame.h"
#include "CodeAttr.h"
#include "../common/utils.h"
#include "ThreadLocalStorage.h"

#include <cstring>
#include <iostream>
#include <memory>

#undef LOG_TAG
#define LOG_TAG "Method"
#include "../common/Logger.h"

using namespace std;
using namespace myvm;

namespace myvm {

Method::Method(ClassInfo* owner,
    uint16_t flags,
    uint16_t name,
    uint16_t desc,
    vector<shared_ptr<AttributeInfo>> *attrs) {
    mOwnerClazz = owner;
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

void Method::invoke(shared_ptr<Frame> frame) {
    if (isAbstract()) {
        LOGW("Can't invoke abstract method!");
        return;
    }
    if (mCodeAttr == nullptr) {
        LOGW("No code for this method!");
        return;
    }

    shared_ptr<OperandStack> stack = ThreadLocalStorage::getInstance()->getStack();
    stack->grow(mCodeAttr->maxStack);

    // interprete the code
    uint8_t depth = frame->getDepth();
    uint8_t *code = mCodeAttr->code;
    uint8_t *codeEnd = code + mCodeAttr->codeLength;
    while (code < codeEnd) {
        LOGI("%sinterprete code: %d", indent(depth), (int32_t)*code);
        unique_ptr<Instruction> instruction(Instruction::interpreteCode(code));
        if (instruction == nullptr) {
            LOGW("Invalid instruction!");
        }
        instruction->run(frame.get());
        code += instruction->length();
    }
}

bool Method::isAbstract() const {
    return (accessFlags & METHOD_ACC_ABSTRACT) != 0;
}

bool Method::isStatic() const {
    return (accessFlags & METHOD_ACC_STATIC) != 0;
}

bool Method::isPublic() const {
    return (accessFlags & METHOD_ACC_PUBLIC) != 0;
}

bool Method::isPrivate() const {
    return (accessFlags & METHOD_ACC_PRIVATE) != 0;
}

bool Method::isProtected() const {
    return (accessFlags & METHOD_ACC_PROTECTED) != 0;
}

bool Method::isConstructor() const {
    return mConstructor;
}

bool Method::isClassConstructor() const {
    return mClassConstructor;
}

Method* Method::loadFromFile(ClassInfo *owner, shared_ptr<FileReader> fileReader) {
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
    owner->printConstantInfo(nameIndex);

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

    LOGI("Have %d attributes!", attributeCount);
    auto attributes = new vector<shared_ptr<AttributeInfo>>();
    if (attributeCount > 0) {
        for (int i = 0; i < attributeCount; i++) {
            LOGI("Load attribute #%d", i);
            AttributeInfo *attrInfo = AttributeFactory::loadFromFile(owner, fileReader);
            if (attrInfo == nullptr) {
                LOGW("Load attribute #%d failed!", i);
                return nullptr;
            }
            attributes->push_back(shared_ptr<AttributeInfo>(attrInfo));
            LOGI("Load attribute #%d complete!", i);
        }
    }
    return new Method(owner, accessFlags, nameIndex, descriptorIndex, attributes);
}

bool Method::match(ConstantNameAndType* nameAndType) {
    // TODO: check the same class
    return nameIndex == nameAndType->nameIndex
        && descriptorIndex == nameAndType->descriptorIndex;
}

bool Method::match(ConstantUtf8* methodName, ConstantUtf8* methodDesc) {
    return methodName->equals(mName) && methodDesc->equals(mDescriptor);
}

bool Method::isMainEntry() const {
    return mMainMethod;
}

void Method::resolve(ClassInfo *clazz) {
    const char* name = clazz->getUtf8ConstantName(nameIndex);
    mName = std::move(std::string(name));

    const char* desc = clazz->getUtf8ConstantName(descriptorIndex);
    mDescriptor = std::move(std::string(desc));

    if (strncmp(name, "main", 4) == 0) {
        const char* mainMethodArgs = "([Ljava/lang/String;)V";
        mMainMethod = isPublic() && isStatic()
            && strncmp(desc, mainMethodArgs, strlen(mainMethodArgs)) == 0;
    } else if (strncmp(name, "<init>", 6) == 0) {
        mConstructor = true;
    } else if (strncmp(name, "<clinit>", 8) == 0) {
        mClassConstructor = true;
    }

    for (auto attr : mAttributes) {
        if (attr->attrType == ATTR_CODE) {
            mCodeAttr = dynamic_pointer_cast<CodeAttr>(attr);
            break;
        }
    }

    if (desc == nullptr) {
        return;
    }
}

ClassInfo* Method::getOwnerClass() {
    return mOwnerClazz;
}

shared_ptr<CodeAttr> Method::getCodeAttr() {
    return mCodeAttr;
}

void Method::resolveParaListAndType() {
    size_t splitPos = mDescriptor.find_last_of(')');
    size_t paraListStart = mDescriptor.find_first_of('(');
    string paraList = mDescriptor.substr(paraListStart, splitPos);
    string returnType = mDescriptor.substr(splitPos);

    for (uint16_t offset = 0; offset < paraList.size();) {
        shared_ptr<TypeInfo> paraType = TypeInfo::parseFrom(paraList, offset);
        if (paraType == nullptr) {
            // ERROR
            return;
        }
        mArgsList.push_back(paraType);
        offset += paraType->descLength();
    }

    mReturnType = TypeInfo::parseFrom(returnType, 0);
}

const string& Method::getName() const {
    return mName;
}

const string& Method::getDesc() const {
    return mDescriptor;
}

}
