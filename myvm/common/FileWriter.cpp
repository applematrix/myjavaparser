#include "FileWriter.h"
#include <zip.h>

using namespace myvm;

namespace myvm {

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
    mOut << message << std::endl;
}

}