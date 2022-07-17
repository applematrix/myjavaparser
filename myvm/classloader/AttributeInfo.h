#ifndef _ATTRIBUTE_INFO_H_
#define _ATTRIBUTE_INFO_H_

#include "common/types.h"
namespace myvm {

struct AttributeInfo {
    uint16_t nameIndex;
    uint32_t length;
};

struct ConstantValueAttr: public AttributeInfo {
    uint16_t constantValueIndex;
};

struct ConstantValueAttr: public AttributeInfo {
    uint16_t constantValueIndex;
};

struct CodeAttr: public AttributeInfo {
    uint16_t maxStack;
    uint16_t maxLocals;
    uint32_t codeLength;
    uint8_t *code;
    uint16_t exceptionTableLength;
    struct ExceptionTable {
        uint16_t startPc;
        uint16_t endPc;
        uint16_t handlerPc;
        uint16_t catchType;
    } *exceptionTable;
    uint16_t attrCounts;
    AttributeInfo *attributes;
};

struct StackMapTableAttr: public AttributeInfo {
    uint16_t entryCount;
    // TODO:
    uint8_t entries;
};

struct ExceptionAttr: public AttributeInfo {
    uint16_t exceptionCount;
    uint16_t *exceptionIndexTable;
};

struct InnerClassAttr: public AttributeInfo {
    uint16_t numberOfClasses;
    struct {
        uint16_t innerClassInfoIndex;
        uint16_t outerClassInfoIndex;
        uint16_t innerNameIndex;
        uint16_t innerClassAccessFlags;
    } *classes;
};

struct EnclosingMethodAttr: public AttributeInfo {
    uint16_t classIndex;
    uint16_t methodIndex;
};

typedef AttributeInfo SyntheticAttr;

struct SignatureAttr: public AttributeInfo {
    uint16_t singatureIndex;
};

struct SourceFileAttr: public AttributeInfo {
    uint16_t sourceFileIndex;
};

struct SourceDebugExtensionAttr: public AttributeInfo {
    uint8_t debugExtension[0];
};

struct LineNumberTableAttr: public AttributeInfo {
    struct {
        uint16_t startPc;
        uint16_t lineNumber;
    } lineNumberTable[0];
};

struct LocalVariableTableAttr: public AttributeInfo {
    struct {
        uint16_t startPc;
        uint16_t length;
        uint16_t nameIndex;
        uint16_t descriptorIndex;
        uint16_t index;
    } localVariableTable[0];
};

struct LocalVariableTypeTableAttr: public AttributeInfo {
    struct {
        uint16_t startPc;
        uint16_t length;
        uint16_t nameIndex;
        uint16_t signatureIndex;
        uint16_t index;
    } localVariableTypeTable[0];
};

typedef AttributeInfo DeprecatedAttr;

struct RuntimeVisibleAnnotations: public AttributeInfo {
    uint16_t annotationNums;
    // TODO:
    // annotation annotations[0];
};

struct BootstrapMethodsAttr: public AttributeInfo {
    uint16_t bootstrppMethodNum;
    struct {
        uint16_t methodRef;
        uint16_t argc;
        uint16_t *args;
    } bootstrapMethods[0];
};

struct MethodParametersAttr: public AttributeInfo {
    uint8_t paramCount;
    struct {
        uint16_t nameIndex;
        uint16_t accessFlags;
    } parameters[0];
};

struct ModuleAttr: public AttributeInfo {
    uint8_t paramCount;
    uint16_t moduleNameIndex;
    uint16_t moduleFlags;
    uint16_t moduleVersionIndex;

    uint16_t requiresCount;
    struct {
        uint16_t requiresIndex;
        uint16_t requiresFlags;
        uint16_t requiresVersionIndex;
    } *requires;

    uint16_t exportsCount;
    struct {
        uint16_t exportsIndex;
        uint16_t exportsFlags;
        uint16_t exportsToIndexCount;
        uint16_t *exportsToIndex;
    } *exports;

    // TODO:
};

struct ModulePackageAttr: public AttributeInfo {
    uint16_t packageCount;
    uint16_t packageIndex[0];
};

struct ModuleMainClassAttr: public AttributeInfo {
    uint16_t mainClassIndex;
};

struct NestHostAttr: public AttributeInfo {
    uint16_t hostClassIndex;
};

struct NestMemberAttr: public AttributeInfo {
    uint16_t classNum;
    uint16_t classes[0];
};

struct RecordAttr: public AttributeInfo {
    uint16_t componentCount;
    struct RecordComponentInfo {
        uint16_t nameIndex;
        uint16_t descriptorIndex;
        uint16_t attributeCount;
        AttributeInfo * attributeInfo;
    } *components;
};

typedef NestMemberAttr PermittedSubClassAttr;

}

#endif