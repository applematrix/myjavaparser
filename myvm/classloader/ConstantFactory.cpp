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
        case ConstantTag::Utf8:
            return createConstantUtf8(fileReader);
        case ConstantTag::Integer:
        case ConstantTag::Float:
            return createConstantIntegerOrFloat(tag, fileReader);
        case ConstantTag::Long:
        case ConstantTag::Double:
            return createConstantLongOrDouble(tag, fileReader);
        case ConstantTag::String:
            return createConstantString(fileReader);
        case ConstantTag::FieldRef:
        case ConstantTag::MethodRef:
        case ConstantTag::InterfaceMethodRef:
            return createConstantRef(tag, fileReader);
        case ConstantTag::MethodHandle:
            return createMethodHandle(tag, fileReader);
        case ConstantTag::Class:
            return createClass(tag, fileReader);
        case ConstantTag::NameAndType:
            return createNameAndType(fileReader);
        default:break;
    }
    return nullptr;
}

}