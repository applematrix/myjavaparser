#ifndef _ATTRIBUTE_INFO_H_
#define _ATTRIBUTE_INFO_H_

#include "common/types.h"
#include "FileReader.h"

namespace myvm {

enum AttributeType {
    ConstantValue = 1,
    Code,
    
};

struct Attribute {
};

struct AttributeInfo {
    uint16_t nameIndex;
    uint32_t length;

    AttributeInfo() {
        nameIndex = 0;
        length = 0;
    }

    AttributeInfo(FileReader* fileReader) {
        fileReader->readUint16(nameIndex);
        fileReader->readUint32(length);
    }
};

struct ConstantValueAttr: public AttributeInfo {
    uint16_t constantValueIndex;

    ConstantValueAttr(FileReader *fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(constantValueIndex);
    }
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
    AttributeInfo **attributes;

    CodeAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(maxStack);
        fileReader->readUint16(maxStack);
        fileReader->readUint32(codeLength);

        code = new uint8_t[codeLength];
        fileReader->read(code, codeLength);

        loadExceptionTable(fileReader);
        fileReader->readUint16(attrCounts);

        attributes = new AttributeInfo*[attrCounts];
        for (int i = 0; i < attrCounts; i++) {
            attributes[i] = AttributeFactory::loadFromFile(fileReader);
        }
    }

    void loadExceptionTable(FileReader* fileReader) {
        fileReader->readUint16(exceptionTableLength);
        exceptionTable = new ExceptionTable[exceptionTableLength];

        for (int i = 0; i < exceptionTableLength; i++) {
            fileReader->readUint16(exceptionTable[i].startPc);
            fileReader->readUint16(exceptionTable[i].endPc);
            fileReader->readUint16(exceptionTable[i].handlerPc);
            fileReader->readUint16(exceptionTable[i].catchType);
        }        
    }
};

struct StackMapTableAttr: public AttributeInfo {
    uint16_t entryCount;
    // TODO:
    uint8_t entries;

    StackMapTableAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
    }
};

struct ExceptionAttr: public AttributeInfo {
    uint16_t exceptionCount;
    uint16_t *exceptionIndexTable;

    ExceptionAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(exceptionCount);

        exceptionIndexTable = new uint16_t[exceptionCount];
        for (int i = 0; i < exceptionCount; i++) {
            fileReader->readUint16(exceptionIndexTable[i]);
        }
    }
};

struct InnerClassAttr: public AttributeInfo {
    uint16_t numberOfClasses;
    struct Clazz {
        uint16_t innerClassInfoIndex;
        uint16_t outerClassInfoIndex;
        uint16_t innerNameIndex;
        uint16_t innerClassAccessFlags;
    } *classes;

    InnerClassAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(numberOfClasses);

        classes = new Clazz[numberOfClasses];
        for (int i = 0; i < numberOfClasses; i++) {
            fileReader->readUint16(classes[i].innerClassAccessFlags);
        }
    }
};

struct EnclosingMethodAttr: public AttributeInfo {
    uint16_t classIndex;
    uint16_t methodIndex;

    EnclosingMethodAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(classIndex);
        fileReader->readUint16(methodIndex);
    }
};

typedef AttributeInfo SyntheticAttr;

struct SignatureAttr: public AttributeInfo {
    uint16_t singatureIndex;

    SignatureAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(singatureIndex);
    }
};

struct SourceFileAttr: public AttributeInfo {
    uint16_t sourceFileIndex;

    SourceFileAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(sourceFileIndex);
    }
};

struct SourceDebugExtensionAttr: public AttributeInfo {
    uint8_t *debugExtension;

    SourceDebugExtensionAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        debugExtension = new uint8_t[length];
        fileReader->read(debugExtension, length);
    }
};

struct LineNumberTableAttr: public AttributeInfo {
    uint16_t tableLen;
    struct LineNumber {
        uint16_t startPc;
        uint16_t lineNumber;
    } * lineNumberTable;

    LineNumberTableAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(tableLen);

        lineNumberTable = new LineNumber[tableLen];
        for (int i = 0; i < tableLen; i++) {
            fileReader->read(lineNumberTable[i].startPc);
            fileReader->read(lineNumberTable[i].lineNumber);
        }
    }
};

struct LocalVariableTableAttr: public AttributeInfo {
    uint16_t tableLen;
    struct LocalVariable {
        uint16_t startPc;
        uint16_t length;
        uint16_t nameIndex;
        uint16_t descriptorIndex;
        uint16_t index;
    } *localVariableTable;

    LocalVariableTableAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(tableLen);

        localVariableTable = new LocalVariable[tableLen];
        for (int i = 0; i < tableLen; i++) {
            fileReader->read(localVariableTable[i].startPc);
            fileReader->read(localVariableTable[i].length);
            fileReader->read(localVariableTable[i].nameIndex);
            fileReader->read(localVariableTable[i].descriptorIndex);
            fileReader->read(localVariableTable[i].index);
        }
    }
};

struct LocalVariableTypeTableAttr: public AttributeInfo {
    uint16_t tableLen;
    struct LocalVariableType {
        uint16_t startPc;
        uint16_t length;
        uint16_t nameIndex;
        uint16_t signatureIndex;
        uint16_t index;
    } *localVariableTypeTable;

    LocalVariableTypeTableAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(tableLen);

        localVariableTypeTable = new LocalVariableType[tableLen];
        for (int i = 0; i < tableLen; i++) {
            fileReader->read(localVariableTypeTable[i].startPc);
            fileReader->read(localVariableTypeTable[i].length);
            fileReader->read(localVariableTypeTable[i].nameIndex);
            fileReader->read(localVariableTypeTable[i].signatureIndex);
            fileReader->read(localVariableTypeTable[i].index);
        }
    }
};

typedef AttributeInfo DeprecatedAttr;

struct RuntimeVisibleAnnotations: public AttributeInfo {
    uint16_t annotationNums;
    // TODO:
    // annotation annotations[0];
};

struct BootstrapMethodsAttr: public AttributeInfo {
    uint16_t bootstrapMethodNum;
    struct BootstrapMethod {
        uint16_t methodRef;
        uint16_t argc;
        uint16_t *args;
    } **bootstrapMethods;

    BootstrapMethodsAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(bootstrapMethodNum);

        bootstrapMethods = new BootstrapMethod*[bootstrapMethodNum];
        for (int i = 0; i < bootstrapMethodNum; i++) {
            fileReader->readUint16(bootstrapMethods[i]->methodRef);
            fileReader->readUint16(bootstrapMethods[i]->argc);
        }
    }
};

struct MethodParametersAttr: public AttributeInfo {
    uint8_t paramCount;
    struct Parameter {
        uint16_t nameIndex;
        uint16_t accessFlags;
    } *parameters;

    MethodParametersAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint8(paramCount);

        parameters = new Parameter[paramCount];
        for (int i = 0; i < paramCount; i++) {
            fileReader->readUint16(parameters[i].nameIndex);
            fileReader->readUint16(parameters[i].accessFlags);
        }
    }
};

struct ModuleAttr: public AttributeInfo {
    uint8_t paramCount;
    uint16_t moduleNameIndex;
    uint16_t moduleFlags;
    uint16_t moduleVersionIndex;

    uint16_t requiresCount;
    struct Require {
        uint16_t requiresIndex;
        uint16_t requiresFlags;
        uint16_t requiresVersionIndex;
    } *requires;

    uint16_t exportsCount;
    struct Export {
        uint16_t exportsIndex;
        uint16_t exportsFlags;
        uint16_t exportsToIndexCount;
        uint16_t *exportsToIndex;
    } *exports;

    uint16_t opensCount;
    struct Export {
        uint16_t opensIndex;
        uint16_t opensFlags;
        uint16_t opensToIndexCount;
        uint16_t *opensToIndex;
    } *opens;

    uint16_t usesCount;
    uint16_t *usesIndex;

    uint16_t providesCount;
    struct Provide {
        uint16_t providesIndex;
        uint16_t providesWithCount;
        uint16_t *providesWithIndex;
    } *provides;

    // constructor
    ModuleAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint8(paramCount);
        fileReader->readUint16(moduleNameIndex);
        fileReader->readUint16(moduleFlags);
        fileReader->readUint16(moduleVersionIndex);

        fileReader->readUint16(requiresCount);
        if (requiresCount > 0) {
            requires = new Require[requiresCount];
            for (int i = 0; i < requiresCount; i++) {
                fileReader->readUint16(requires[i].requiresIndex);
                fileReader->readUint16(requires[i].requiresFlags);
                fileReader->readUint16(requires[i].requiresVersionIndex);
            }
        }

        fileReader->readUint16(exportsCount);
        if (exportsCount > 0) {
            exports = new Export[exportsCount];
            for (int i = 0; i < exportsCount; i++) {
                fileReader->readUint16(exports[i].exportsIndex);
                fileReader->readUint16(exports[i].exportsFlags);
                fileReader->readUint16(exports[i].exportsToIndexCount);

                if (exports[i].exportsToIndexCount > 0) {
                    exports[i].exportsToIndex = new uint16_t[exports[i].exportsToIndexCount];
                    for (int j = 0; j < exports[i].exportsToIndexCount; j++) {
                        fileReader->readUint16(exports[i].exportsToIndex[j]);
                    }
                }
            }
        }

        fileReader->readUint16(opensCount);
        if (opensCount > 0) {
            opens = new Export[opensCount];
            for (int i = 0; i < opensCount; i++) {
                fileReader->readUint16(opens[i].opensIndex);
                fileReader->readUint16(opens[i].opensFlags);
                fileReader->readUint16(opens[i].opensToIndexCount);

                if (opens[i].opensToIndexCount > 0) {
                    opens[i].opensToIndex = new uint16_t[opens[i].opensToIndexCount];
                    for (int j = 0; j < opens[i].opensToIndexCount; j++) {
                        fileReader->readUint16(opens[i].opensToIndex[j]);
                    }
                }
            }
        }

        fileReader->readUint16(usesCount);
        if (usesCount > 0) {
            usesIndex = new uint16_t[usesCount];
            for (int i = 0; i < usesCount; i++) {
                fileReader->readUint16(usesIndex[i]);
            }
        }

        fileReader->readUint16(providesCount);
        if (providesCount > 0) {
            provides = new Provide[providesCount];
            for (int i = 0; i < providesCount; i++) {
                fileReader->readUint16(provides[i].providesIndex);
                fileReader->readUint16(provides[i].providesWithCount);
                if (provides[i].providesWithCount == 0) {
                    continue;
                }
                provides[i].providesWithIndex = new uint16_t[provides[i].providesWithCount];
                for (int j = 0; j < provides[i].providesWithCount; j++) {
                    fileReader->readUint16(provides[i].providesWithIndex[j]);
                }
            }
        }
    }
};

struct ModulePackageAttr: public AttributeInfo {
    uint16_t packageCount;
    uint16_t *packageIndex;

    ModulePackageAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(packageCount);

        if (packageCount > 0) {
            packageIndex = new uint16_t[packageCount];
            for (int i = 0; i < packageCount; i++) {
                fileReader->readUint16(packageIndex[i]);
            }
        }
    }
};

struct ModuleMainClassAttr: public AttributeInfo {
    uint16_t mainClassIndex;

    ModuleMainClassAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(mainClassIndex);
    }
};

struct NestHostAttr: public AttributeInfo {
    uint16_t hostClassIndex;

    NestHostAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(hostClassIndex);
    }
};

struct NestMemberAttr: public AttributeInfo {
    uint16_t classNum;
    uint16_t *classes;

    NestMemberAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(classNum);
        for (int i = 0; i < classNum; i++) {
            fileReader->readUint16(classes[i]);
        }
    }
};

struct RecordAttr: public AttributeInfo {
    uint16_t componentCount;
    struct RecordComponentInfo {
        uint16_t nameIndex;
        uint16_t descriptorIndex;
        uint16_t attributeCount;
        AttributeInfo **attributeInfo;
    } *components;

    RecordAttr(FileReader* fileReader) : AttributeInfo(fileReader) {
        fileReader->readUint16(componentCount);
        for (int i = 0; i < componentCount; i++) {
            fileReader->readUint16(components[i].nameIndex);
            fileReader->readUint16(components[i].descriptorIndex);
            fileReader->readUint16(components[i].attributeCount);

            if (components[i].attributeCount == 0) {
                continue;
            }
            components[i].attributeInfo = new AttributeInfo*[components[i].attributeCount];
            for (int j = 0; j < components[i].attributeCount; j++) {
                components[i].attributeInfo[j] = AttributeFactory::loadFromFile(fileReader);
            }
        }
    }
};

typedef NestMemberAttr PermittedSubClassAttr;

}

#endif