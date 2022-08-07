/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include "FileReader.h"

namespace myvm {

FileReader::FileReader() {
    mSize = 0;
    mOffset = 0;
}

FileReader::~FileReader() {
}

FileReader::FileReader(const char* path) {
    mClassFilePath = path;
    mSize = 0;
    mOffset = 0;
    loadFile();
}

int FileReader::read(void *buffer, int32_t dataSize) {
    if (buffer == nullptr) {
        return -1;
    }
    if (mOffset + dataSize > mSize) {
        return -1;
    }
    int32_t readSize = fread(buffer, sizeof(int8_t), dataSize, mClassFile);
    mOffset += readSize;
    return readSize == dataSize ? 0 : -1;
}

// private functions
void FileReader::loadFile() {
    mClassFile = fopen(mClassFilePath.c_str(), "rb");
    if (mClassFile == nullptr) {
        return;
    }
    int64_t fileSize = fseek(mClassFile, 0, SEEK_END);
    mSize = ftell(mClassFile);
    fseek(mClassFile, 0, SEEK_SET);
}

bool FileReader::valid() {
    if (mClassFile == NULL) {
        return false;
    }
    if (mOffset > mSize) {
        return false;
    }
    return true;
}

}