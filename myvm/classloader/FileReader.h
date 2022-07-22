#ifndef _FILE_READER_H_
#define _FILE_READER_H_
#include <stdio.h>
#include <string>
#include "stdint.h"

using namespace std;

namespace myvm {
class FileReader {
public:
    FileReader();
    FileReader(const char* path);
    ~FileReader();
    inline uint64_t getOffset() { return mOffset;}
    inline uint64_t getSize() {return mSize;}
    inline void markPosition() {mMarkPosition = mOffset;}
    inline void unmarkPosition() {mMarkPosition = 0;}

    int readInt32(int32_t &val) {
        return read<int32_t>(val);
    }

    int readInt16(int16_t &val) {
        return read<int16_t>(val);
    }

    int readInt8(int8_t &val) {
        return read<int8_t>(val);
    }

    int readUint32(uint32_t &val) {
        return read<uint32_t>(val);
    }

    int readUint16(uint16_t &val) {
        return read<uint16_t>(val);
    }
    int readUint8(uint8_t &val) {
        return read<uint8_t>(val);
    }

    template<class T> int read(T &val) {
        int8_t dataSize = sizeof(val);
        if (mOffset + dataSize > mSize) {
            return -1;
        }
        T tmpVal = 0;
        int readSize = fread(&tmpVal, 1, dataSize, mClassFile);
        mOffset += dataSize;
        if (readSize != dataSize) return -1;

        // litte-endian
        val = 0;
        while (dataSize-- > 0) {
            val <<= 8;
            val |= (tmpVal & 0xff);
            tmpVal >>= 8;
        }
        return 0;
    }
    
    int read(void *buffer, int32_t readSize);
private:
    void loadFile();
    bool valid();
private:
    FILE *mClassFile;
    std::string mClassFilePath;
    uint64_t mOffset;
    uint64_t mMarkPosition;
    uint64_t mSize;
};

}
#endif