#include "RegularFileReader.h"

using namespace myvm;

namespace myvm {

RegularFileReader::RegularFileReader() {

}

RegularFileReader::~RegularFileReader() {

}

RegularFileReader::RegularFileReader(const char* path) {
    mClassFile = fopen(mClassFilePath.c_str(), "rb");
    if (mClassFile == nullptr) {
        return;
    }
    int64_t fileSize = fseek(mClassFile, 0, SEEK_END);
    mSize = ftell(mClassFile);
    fseek(mClassFile, 0, SEEK_SET);
}

size_t RegularFileReader::readFromFile(void *buffer, size_t size) {
    size_t readSize = fread(buffer, 1, size, mClassFile);
    return readSize;
}

}