#include "ClassFileReader.h"

using namespace myvm;

namespace myvm {

ClassFileReader::ClassFileReader() {

}

ClassFileReader::~ClassFileReader() {

}

ClassFileReader::ClassFileReader(const char* path) {
    mClassFilePath = path;
    mClassFile = fopen(mClassFilePath.c_str(), "rb");
    if (mClassFile == nullptr) {
        return;
    }
    int64_t fileSize = fseek(mClassFile, 0, SEEK_END);
    mSize = ftell(mClassFile);
    fseek(mClassFile, 0, SEEK_SET);
    mOffset = 0;
}

size_t ClassFileReader::readFromFile(void *buffer, size_t size) {
    if (mOffset + size > mSize) {
        return -1;
    }
    size_t readSize = fread(buffer, 1, size, mClassFile);
    mOffset += size;
    return readSize;
}

void ClassFileReader::skip(uint32_t bytes) {
    mOffset += bytes;
    int64_t fileSize = fseek(mClassFile, mOffset, SEEK_SET);
}

}