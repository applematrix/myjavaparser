#pragma once

#include <stdio.h>
#include <string>
#include "../classloader/FileReader.h"

using namespace std;

namespace myvm {

class RegularFileReader : public FileReader {
public:
    RegularFileReader();
    RegularFileReader(const char* path);
    ~RegularFileReader();
    virtual size_t readFromFile(void *buffer, size_t size);
private:
    FILE *mClassFile;
    std::string mClassFilePath;
    uint64_t mSize;
};

}