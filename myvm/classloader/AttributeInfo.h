/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _ATTRIBUTE_INFO_H_
#define _ATTRIBUTE_INFO_H_

#include "FileReader.h"
#include "ClassInfo.h"
#include "AttributeFactory.h"

#include <vector>
#include <memory>

using namespace myvm;

namespace myvm {

class ClassInfo;
class AttributeFactory;

enum AttributeType {
    ATTR_CONSTANT_VALUE = 0,
    ATTR_CODE,
    ATTR_STACK_MAP_TABLE,
    ATTR_EXCEPTIONS,
    ATTR_INNER_CLASSES,
    ATTR_ENCLOSING_METHOD,
    ATTR_SYNTHETIC,
    ATTR_SIGNATURE,
    ATTR_SOURCE_FILE,
    ATTR_SOURCE_DEBUG_EXTENSION,
    ATTR_LINE_NUMBER_TABLE,
    ATTR_LOCAL_VARIABLE_TABLE,
    ATTR_LOCAL_VARIABLE_TYPE_TABLE,
    ATTR_DEPRECATED,
    ATTR_RUNTIME_VISIBLE_ANNOTATIONS,
    ATTR_RUNTIME_INVISIBLE_ANNOTATIONS,
    ATTR_ANNOTATION_DEFAULT,
    ATTR_BOOTSTRAP_METHODS,
    ATTR_METHOD_PARAMETERS,
    ATTR_MODULE,
    ATTR_MODULE_PACKAGE,
    ATTR_MODULE_MAIN_CLASS,
    ATTR_NEST_HOST,
    ATTR_NEST_MEMBER,
    ATTR_RECORD,
    ATTR_PERMITTED_SUBCLASS,
};

const char* nameOfAttrType(uint8_t type);
uint8_t getAttributeType(uint8_t *bytes, uint16_t len);

struct AttributeInfo {
    uint16_t nameIndex;
    uint32_t length;
    uint8_t attrType;

    AttributeInfo() {
        nameIndex = 0;
        length = 0;
        attrType = 0;
    }

    virtual ~AttributeInfo() {
    }

    AttributeInfo(FileReader* fileReader, uint8_t type) {
        fileReader->readUint16(nameIndex);
        fileReader->readUint32(length);
        attrType = type;
    }

    AttributeInfo(uint16_t name, uint32_t len, uint8_t type)
        : nameIndex(name), length(len), attrType(type) {
    }
};

struct ConstantValueAttr: public AttributeInfo {
    uint16_t constantValueIndex;

    ConstantValueAttr(FileReader *fileReader) : AttributeInfo(fileReader, ATTR_CONSTANT_VALUE) {
        fileReader->readUint16(constantValueIndex);
    }

    ConstantValueAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_CONSTANT_VALUE) {
        fileReader->readUint16(constantValueIndex);
    }
};

struct StackMapTableAttr: public AttributeInfo {
    uint16_t entryCount = 0;
    // TODO:
    uint8_t entries = 0;

    StackMapTableAttr(uint16_t name, uint32_t len, FileReader* fileReader)
        : AttributeInfo(name, len, ATTR_STACK_MAP_TABLE) {
        // TODO:
        fileReader->skip(this->length);
    }
};

struct ExceptionAttr: public AttributeInfo {
    uint16_t exceptionCount;
    uint16_t *exceptionIndexTable;

    void loadExceptionIndexTable(FileReader* fileReader) {
        fileReader->readUint16(exceptionCount);
        exceptionIndexTable = new uint16_t[exceptionCount];
        for (int i = 0; i < exceptionCount; i++) {
            fileReader->readUint16(exceptionIndexTable[i]);
        }
    }

    ExceptionAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_EXCEPTIONS) {
        loadExceptionIndexTable(fileReader);
    }

    ExceptionAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_EXCEPTIONS) {
        loadExceptionIndexTable(fileReader);
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

    void initialize(FileReader* fileReader) {
        fileReader->readUint16(numberOfClasses);

        classes = new Clazz[numberOfClasses];
        for (int i = 0; i < numberOfClasses; i++) {
            fileReader->readUint16(classes[i].innerClassAccessFlags);
        }
    }

    InnerClassAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_INNER_CLASSES) {
        initialize(fileReader);
    }

    InnerClassAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_INNER_CLASSES) {
        initialize(fileReader);
    }
};

struct EnclosingMethodAttr: public AttributeInfo {
    uint16_t classIndex;
    uint16_t methodIndex;

    EnclosingMethodAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_ENCLOSING_METHOD) {
        fileReader->readUint16(classIndex);
        fileReader->readUint16(methodIndex);
    }

    EnclosingMethodAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_ENCLOSING_METHOD) {
        fileReader->readUint16(classIndex);
        fileReader->readUint16(methodIndex);
    }
};

typedef AttributeInfo SyntheticAttr;

struct SignatureAttr: public AttributeInfo {
    uint16_t singatureIndex;

    SignatureAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_SIGNATURE) {
        fileReader->readUint16(singatureIndex);
    }

    SignatureAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_SIGNATURE) {
        fileReader->readUint16(singatureIndex);
    }
};

struct SourceFileAttr: public AttributeInfo {
    uint16_t sourceFileIndex;

    SourceFileAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_SOURCE_FILE) {
        fileReader->readUint16(sourceFileIndex);
    }

    SourceFileAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_SOURCE_FILE) {
        fileReader->readUint16(sourceFileIndex);
    }
};

struct SourceDebugExtensionAttr: public AttributeInfo {
    uint8_t *debugExtension;

    SourceDebugExtensionAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_SOURCE_DEBUG_EXTENSION) {
        debugExtension = new uint8_t[length];
        fileReader->read(debugExtension, length);
    }

    SourceDebugExtensionAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_SOURCE_DEBUG_EXTENSION) {
        fileReader->read(debugExtension, length);
    }
};

struct LineNumberTableAttr: public AttributeInfo {
    uint16_t tableLen;
    struct LineNumber {
        uint16_t startPc;
        uint16_t lineNumber;
    } * lineNumberTable;

    void initialize(FileReader* fileReader) {
        fileReader->readUint16(tableLen);

        lineNumberTable = new LineNumber[tableLen];
        for (int i = 0; i < tableLen; i++) {
            fileReader->readUint16(lineNumberTable[i].startPc);
            fileReader->readUint16(lineNumberTable[i].lineNumber);
        }
    }

    LineNumberTableAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_LINE_NUMBER_TABLE) {
        initialize(fileReader);
    }

    LineNumberTableAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_LINE_NUMBER_TABLE) {
        initialize(fileReader);
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

    void initialize(FileReader* fileReader) {
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

    LocalVariableTypeTableAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_LOCAL_VARIABLE_TYPE_TABLE) {
        initialize(fileReader);
    }

    LocalVariableTypeTableAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_LOCAL_VARIABLE_TYPE_TABLE) {
        initialize(fileReader);
    }
};

typedef AttributeInfo DeprecatedAttr;

struct RuntimeVisibleAnnotationsAttr: public AttributeInfo {
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

    void initialize(FileReader* fileReader) {
        fileReader->readUint16(bootstrapMethodNum);

        bootstrapMethods = new BootstrapMethod*[bootstrapMethodNum];
        for (int i = 0; i < bootstrapMethodNum; i++) {
            fileReader->readUint16(bootstrapMethods[i]->methodRef);
            fileReader->readUint16(bootstrapMethods[i]->argc);
        }
    }

    BootstrapMethodsAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_BOOTSTRAP_METHODS) {
        initialize(fileReader);
    }

    BootstrapMethodsAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_BOOTSTRAP_METHODS) {
        initialize(fileReader);
    }
};

struct MethodParametersAttr: public AttributeInfo {
    uint8_t paramCount;
    struct Parameter {
        uint16_t nameIndex;
        uint16_t accessFlags;
    } *parameters;

    void initialize(FileReader* fileReader) {
        fileReader->readUint8(paramCount);

        parameters = new Parameter[paramCount];
        for (int i = 0; i < paramCount; i++) {
            fileReader->readUint16(parameters[i].nameIndex);
            fileReader->readUint16(parameters[i].accessFlags);
        }
    }

    MethodParametersAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_METHOD_PARAMETERS) {
        initialize(fileReader);
    }

    MethodParametersAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_METHOD_PARAMETERS) {
        initialize(fileReader);
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
    struct Open {
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
    void initialize(FileReader* fileReader) {
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
            opens = new Open[opensCount];
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

    ModuleAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_MODULE) {
        initialize(fileReader);
    }

    ModuleAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_MODULE) {
        initialize(fileReader);
    }
};

struct ModulePackageAttr: public AttributeInfo {
    uint16_t packageCount;
    uint16_t *packageIndex;

    void initialize(FileReader* fileReader) {
        fileReader->readUint16(packageCount);

        if (packageCount > 0) {
            packageIndex = new uint16_t[packageCount];
            for (int i = 0; i < packageCount; i++) {
                fileReader->readUint16(packageIndex[i]);
            }
        }
    }

    ModulePackageAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_MODULE_PACKAGE) {
        initialize(fileReader);
    }

    ModulePackageAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_MODULE_PACKAGE) {
        initialize(fileReader);
    }
};

struct ModuleMainClassAttr: public AttributeInfo {
    uint16_t mainClassIndex;

    ModuleMainClassAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_MODULE_MAIN_CLASS) {
        fileReader->readUint16(mainClassIndex);
    }

    ModuleMainClassAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_MODULE_MAIN_CLASS) {
        fileReader->readUint16(mainClassIndex);
    }    
};

struct NestHostAttr: public AttributeInfo {
    uint16_t hostClassIndex;

    NestHostAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_NEST_HOST) {
        fileReader->readUint16(hostClassIndex);
    }

    NestHostAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_NEST_HOST) {
        fileReader->readUint16(hostClassIndex);
    }
};

struct NestMemberAttr: public AttributeInfo {
    uint16_t classNum;
    uint16_t *classes;

    NestMemberAttr(FileReader* fileReader) : AttributeInfo(fileReader, ATTR_NEST_MEMBER) {
        fileReader->readUint16(classNum);
        for (int i = 0; i < classNum; i++) {
            fileReader->readUint16(classes[i]);
        }
    }

    NestMemberAttr(uint16_t name, uint32_t len, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_NEST_MEMBER) {
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

    void initialize(ClassInfo *classFileInfo, FileReader* fileReader);

    RecordAttr(ClassInfo *classFileInfo, FileReader* fileReader) 
        : AttributeInfo(fileReader, ATTR_RECORD) {
        initialize(classFileInfo, fileReader);
    }

    RecordAttr(uint16_t name, uint32_t len, ClassInfo *classFileInfo, FileReader *fileReader)
        : AttributeInfo(name, len, ATTR_RECORD) {
        initialize(classFileInfo, fileReader);
    }
};

typedef NestMemberAttr PermittedSubClassAttr;

}

#endif