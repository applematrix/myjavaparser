#include "FileWriter.h"
#include <zip.h>
#include <iostream>
#include <fstream>

using namespace myvm;

namespace myvm {

FileWriter::FileWriter() {
    useStdOut = true;
}

FileWriter::FileWriter(const char* path) {
    mOut.open(path, std::ios::app);

    // Test
    int error;
    zip_t* zip = zip_open(path, ZIP_RDONLY, &error);
}

FileWriter::~FileWriter() {
    mOut.close();
}

void FileWriter::writeMessage(const string& message) {
    if (useStdOut) {
        std::cout << message << std::endl;
    } else {
        mOut << message << std::endl;
    }
}

}