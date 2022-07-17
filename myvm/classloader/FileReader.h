#ifndef _FILE_READER_H_
#define _FILE_READER_H_
#include <stdio.h>
#include "common/types.h"

namespace myvm {
class FileReader {
public:
    FileReader();
    FileReader(const char* path);
    ~FileReader();
    
private:
    FILE *mFile;
    uint64_t mOffset;
    uint64_t mMarkPosition;
    uint64_t mSize;
};

}
#endif