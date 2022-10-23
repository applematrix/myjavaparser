#pragma once

#include <stdio.h>
#include <string>
#include "../classloader/FileReader.h"

using namespace std;

namespace myvm {

class ClassFileReader : public FileReader {
public:
    ClassFileReader();
    ClassFileReader(const char* path);
    ~ClassFileReader();
    virtual size_t readFromFile(void *buffer, size_t size);
    virtual uint64_t getOffset() override { return mOffset;}
    virtual uint64_t getSize() override {return mSize;}
    virtual void skip(uint32_t bytes);
private:
    FILE *mClassFile;
    std::string mClassFilePath;
    uint64_t mSize;
    uint64_t mOffset;
};

}