#include "FileWriter.h"

using namespace myvm;

namespace myvm {

FileWriter::FileWriter(const char* path) {
    mOut.open(path, std::ios::app);
}

FileWriter::~FileWriter() {
    mOut.close();
}

void FileWriter::writeMessage(const string& message) {
    mOut << message << std::endl;
}

}