/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include <iostream>

#include "AttributeFactory.h"
#include "AttributeInfo.h"

using namespace myvm;
using namespace std;

namespace myvm {

struct AttributeMap{
    const char* attributeName;
    uint8_t attributeType;
};

static uint8_t getAttributeType(ClassFileInfo* classInfo, uint16_t nameIndex) {
    ConstantInfo* constantInfo = classInfo->getConstantAt(nameIndex);
    if (constantInfo == nullptr) {
        return -1;
    }

    uint8_t tag = constantInfo->tag;
    if (tag != CONSTANT_UTF8) {
        return -1;
    }
    ConstantUtf8 *utf8Info = (ConstantUtf8 *)constantInfo;
    return getAttributeType(utf8Info->bytes, utf8Info->length);
}

AttributeInfo* AttributeFactory::loadFromFile(ClassFileInfo* classInfo, FileReader* fileReader) {
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
    std::cout << "Load attribute type:" << (int32_t)type << ", name:" << nameOfAttrType(type)
        << ", attribute length: " << attrLength << endl;
    switch (type)
    {
        case AttributeType::ConstantValue:
            return new ConstantValueAttr(nameIndex, attrLength, fileReader);
        case AttributeType::Code:
            return new CodeAttr(nameIndex, attrLength, classInfo, fileReader);
        case AttributeType::StackMapTable:
            // TODO:return new StackMapTableAttr()
        case AttributeType::Exceptions:
            return new ExceptionAttr(nameIndex, attrLength, fileReader);
        case AttributeType::InnerClasses:
            return new InnerClassAttr(nameIndex, attrLength, fileReader);
        case AttributeType::EnclosingMethod:
            return new EnclosingMethodAttr(nameIndex, attrLength, fileReader);
        case AttributeType::Synthetic:
            return new SyntheticAttr(fileReader);
        case AttributeType::Signature:
            return new SignatureAttr(nameIndex, attrLength, fileReader);
        case AttributeType::SourceFile:
            return new SourceFileAttr(nameIndex, attrLength, fileReader);
        case AttributeType::SourceDebugExtension:
            return new SourceDebugExtensionAttr(nameIndex, attrLength, fileReader);
        case AttributeType::LineNumberTable:
            return new LineNumberTableAttr(nameIndex, attrLength, fileReader);
        case AttributeType::LocalVariableTable:
            return new LocalVariableTableAttr(nameIndex, attrLength, fileReader);
        case AttributeType::LocalVariableTypeTable:
            return new LocalVariableTypeTableAttr(nameIndex, attrLength, fileReader);
        case AttributeType::Deprecated:
            //return new DeprecatedAttr(nameIndex, attrLength, fileReader);
        case AttributeType::RuntimeVisibleAnnotations:
            //return new RuntimeVisibleAnnotationsAttr(nameIndex, attrLength, fileReader);
        case AttributeType::RuntimeInvisibleAnnotations:
            //return new RuntimeVisibleAnnotations(nameIndex, attrLength, fileReader);
        case AttributeType::AnnotationDefault:
        case AttributeType::BootstrapMethods:
            return new BootstrapMethodsAttr(nameIndex, attrLength, fileReader);
        case AttributeType::MethodParameters:
            return new MethodParametersAttr(nameIndex, attrLength, fileReader);
        case AttributeType::Module:
            return new ModuleAttr(nameIndex, attrLength, fileReader);
        case AttributeType::ModulePackage:
            return new ModulePackageAttr(nameIndex, attrLength, fileReader);
        case AttributeType::ModuleMainClass:
            return new ModuleMainClassAttr(nameIndex, attrLength, fileReader);
        case AttributeType::NestHost:
            return new NestHostAttr(nameIndex, attrLength, fileReader);
        case AttributeType::NestMember:
            return new NestMemberAttr(nameIndex, attrLength, fileReader);
        case AttributeType::Record:
            return new RecordAttr(nameIndex, attrLength, classInfo, fileReader);
        case AttributeType::PermittedSubClass:
            return new PermittedSubClassAttr(nameIndex, attrLength, fileReader);
        default:
        break;
    }
    return nullptr;
}

}