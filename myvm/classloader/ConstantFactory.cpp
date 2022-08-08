/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "ConstantFactory.h"

namespace myvm {

static ConstantInfo* createConstantUtf8(FileReader* fileReader) {
    uint16_t length;
    int status = fileReader->readUint16(length);
    if (status != 0) {
        return nullptr;
    }
    uint8_t* bytes = new uint8_t[length + 1];
    memset(bytes, 0, length + 1);
    status = fileReader->read(bytes, length);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantUtf8(length, bytes);
}

static ConstantInfo* createConstantIntegerOrFloat(uint8_t tag, FileReader* fileReader) {
    uint32_t bytes;
    int status = fileReader->readUint32(bytes);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantInteger(tag, bytes);
}

static ConstantInfo* createConstantLongOrDouble(uint8_t tag, FileReader* fileReader) {
    uint32_t highBytes;
    int status = fileReader->readUint32(highBytes);
    if (status != 0) {
        return nullptr;
    }

    uint32_t lowBytes;
    status = fileReader->readUint32(lowBytes);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantLong(tag, highBytes, lowBytes);
}

static ConstantInfo* createConstantString(FileReader* fileReader) {
    uint16_t stringIndex;
    int status = fileReader->readUint16(stringIndex);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantString(stringIndex);
}


static ConstantInfo* createConstantRef(uint8_t tag, FileReader* fileReader) {
    uint16_t classIndex;
    int status = fileReader->readUint16(classIndex);
    if (status != 0) {
        return nullptr;
    }

    uint16_t nameAndTypeIndex;
    status = fileReader->readUint16(nameAndTypeIndex);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantRef(tag, classIndex, nameAndTypeIndex);
}

static ConstantInfo* createMethodHandle(uint8_t tag, FileReader* fileReader) {
    uint16_t kind;
    int status = fileReader->readUint16(kind);
    if (status != 0) {
        return nullptr;
    }

    uint16_t index;
    status = fileReader->readUint16(index);
    if (status != 0) {
        return nullptr;
    }
    return new ConstantMethodHandle(kind, index);
}

static ConstantInfo* createClass(uint8_t tag, FileReader* fileReader) {
    uint16_t index;
    int status = fileReader->readUint16(index);
    if (status != 0) {
        return nullptr;
    }

    return new ConstantClass(tag, index);
}

static ConstantInfo* createNameAndType(FileReader* fileReader) {
    uint16_t nameIndex;
    int status = fileReader->readUint16(nameIndex);
    if (status != 0) {
        return nullptr;
    }

    uint16_t descIndex;
    status = fileReader->readUint16(descIndex);
    if (status != 0) {
        return nullptr;
    }

    return new ConstantNameAndType(nameIndex, descIndex);
}



ConstantInfo* ConstantFactory::loadFromFile(FileReader* fileReader) {
    uint8_t tag = 0;
    int status = fileReader->readUint8(tag);
    if (status != 0) {
        return nullptr;
    }
    switch(tag) {
        case CONSTANT_UTF8:
            return createConstantUtf8(fileReader);
        case CONSTANT_INTEGER:
        case CONSTANT_FLOAT:
            return createConstantIntegerOrFloat(tag, fileReader);
        case CONSTANT_LONG:
        case CONSTANT_DOUBLE:
            return createConstantLongOrDouble(tag, fileReader);
        case CONSTANT_STRING:
            return createConstantString(fileReader);
        case CONSTANT_FIELDREF:
        case CONSTANT_METHODREF:
        case CONSTANT_INTERFACE_METHODREF:
            return createConstantRef(tag, fileReader);
        case CONSTANT_METHODHANDLE:
            return createMethodHandle(tag, fileReader);
        case CONSTANT_CLASS:
            return createClass(tag, fileReader);
        case CONSTANT_NAMEANDTYPE:
            return createNameAndType(fileReader);
        default:break;
    }
    return nullptr;
}

}