#ifndef _JAR_FILE_READER_H_
#define _JAR_FILE_READER_H_

#include <zip.h>
#include <string>
#include "../classloader/FileReader.h"
using namespace std;

namespace myvm {

class JarFileReader : public FileReader {
public:
    JarFileReader();
    virtual ~JarFileReader();
    void open(const char* jarFilePath, const char* classFileName);
    void close();
    virtual size_t readFromFile(void *buffer, size_t size);

private:
    zip_t *mJarFile;
    zip_file * mClassFileInJar;
};

}

#endif