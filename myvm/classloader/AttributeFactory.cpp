/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#undef LOG_TAG
#define LOG_TAG "AttributeFactory"

#include <iostream>

#include "AttributeFactory.h"
#include "AttributeInfo.h"
#include "CodeAttr.h"
#include "LocalVariableTable.h"
#include "../common/Logger.h"

using namespace myvm;
using namespace std;

namespace myvm {

struct AttributeMap{
    const char* attributeName;
    uint8_t attributeType;
};

static uint8_t getAttributeType(ClassInfo* classInfo, uint16_t nameIndex) {
    shared_ptr<ConstantInfo> constantInfo = classInfo->getConstantAt(nameIndex);
    if (constantInfo == nullptr) {
        return -1;
    }

    uint8_t tag = constantInfo->tag;
    if (tag != CONSTANT_UTF8) {
        return -1;
    }
    shared_ptr<ConstantUtf8> utf8Info = dynamic_pointer_cast<ConstantUtf8>(constantInfo);
    return getAttributeType(utf8Info->bytes, utf8Info->length);
}

AttributeInfo* AttributeFactory::loadFromFile(ClassInfo* classInfo, shared_ptr<FileReader> fileReader) {
    uint16_t nameIndex = 0;
    int status = fileReader->readUint16(nameIndex);
    if (status != 0) {
        return nullptr;
    }

    uint32_t attrLength = 0;
    status = fileReader->readUint32(attrLength);
    if (status != 0) {
        return nullptr;
    }

    uint8_t type = getAttributeType(classInfo, nameIndex);
    LOGI("Load attribute type:%d , name:%s , attribute length:%d ", (int32_t)type, nameOfAttrType(type), attrLength);
    switch (type)
    {
        case ATTR_CONSTANT_VALUE:
            return new ConstantValueAttr(nameIndex, attrLength, fileReader);
        case ATTR_CODE:
            return new CodeAttr(nameIndex, attrLength, classInfo, fileReader);
        case ATTR_STACK_MAP_TABLE:
            return new StackMapTableAttr(nameIndex, attrLength, fileReader);
        case ATTR_EXCEPTIONS:
            return new ExceptionAttr(nameIndex, attrLength, fileReader);
        case ATTR_INNER_CLASSES:
            return new InnerClassAttr(nameIndex, attrLength, fileReader);
        case ATTR_ENCLOSING_METHOD:
            return new EnclosingMethodAttr(nameIndex, attrLength, fileReader);
        case ATTR_SYNTHETIC:
            return new SyntheticAttr(fileReader, ATTR_SYNTHETIC);
        case ATTR_SIGNATURE:
            return new SignatureAttr(nameIndex, attrLength, fileReader);
        case ATTR_SOURCE_FILE:
            return new SourceFileAttr(nameIndex, attrLength, fileReader);
        case ATTR_SOURCE_DEBUG_EXTENSION:
            return new SourceDebugExtensionAttr(nameIndex, attrLength, fileReader);
        case ATTR_LINE_NUMBER_TABLE:
            return new LineNumberTableAttr(nameIndex, attrLength, fileReader);
        case ATTR_LOCAL_VARIABLE_TABLE:
            return new LocalVariableTableAttr(nameIndex, attrLength, fileReader);
        case ATTR_LOCAL_VARIABLE_TYPE_TABLE:
            return new LocalVariableTypeTableAttr(nameIndex, attrLength, fileReader);
        case ATTR_DEPRECATED:
            //return new DeprecatedAttr(nameIndex, attrLength, fileReader);
        case ATTR_RUNTIME_VISIBLE_ANNOTATIONS:
            //return new RuntimeVisibleAnnotationsAttr(nameIndex, attrLength, fileReader);
        case ATTR_RUNTIME_INVISIBLE_ANNOTATIONS:
            //return new RuntimeVisibleAnnotations(nameIndex, attrLength, fileReader);
        case ATTR_ANNOTATION_DEFAULT:
        case ATTR_BOOTSTRAP_METHODS:
            return new BootstrapMethodsAttr(nameIndex, attrLength, fileReader);
        case ATTR_METHOD_PARAMETERS:
            return new MethodParametersAttr(nameIndex, attrLength, fileReader);
        case ATTR_MODULE:
            return new ModuleAttr(nameIndex, attrLength, fileReader);
        case ATTR_MODULE_PACKAGE:
            return new ModulePackageAttr(nameIndex, attrLength, fileReader);
        case ATTR_MODULE_MAIN_CLASS:
            return new ModuleMainClassAttr(nameIndex, attrLength, fileReader);
        case ATTR_NEST_HOST:
            return new NestHostAttr(nameIndex, attrLength, fileReader);
        case ATTR_NEST_MEMBER:
            return new NestMemberAttr(nameIndex, attrLength, fileReader);
        case ATTR_RECORD:
            return new RecordAttr(nameIndex, attrLength, classInfo, fileReader);
        case ATTR_PERMITTED_SUBCLASS:
            return new PermittedSubClassAttr(nameIndex, attrLength, fileReader);
        default:
        break;
    }
    return nullptr;
}

}