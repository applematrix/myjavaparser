#ifndef _JAR_CLASS_FILE_READER_H_
#define _JAR_CLASS_FILE_READER_H_

#include <zip.h>
#include <string>
#include "../classloader/FileReader.h"
using namespace std;

namespace myvm {

class JarClassFileReader : public FileReader {
public:
    JarClassFileReader();
    virtual ~JarClassFileReader();
    bool open(string& jarFilePath, string& classFileName);
    void close();
    virtual size_t readFromFile(void *buffer, size_t size);

private:
    zip_t *mJarFile;
    zip_file * mClassFileInJar;
};

}

#endif