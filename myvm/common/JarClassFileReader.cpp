#include "JarClassFileReader.h"
#include <iostream>
#include <errno.h>
#include <string.h>
using namespace std;
using namespace myvm;

const static char* MANIFEST = "META-INF/MANIFEST.MF";

namespace myvm {

JarClassFileReader::JarClassFileReader() {
    mJarFile = nullptr;
    mClassFileInJar = nullptr;
}

JarClassFileReader::~JarClassFileReader() {
    if (mJarFile != nullptr) {
        zip_close(mJarFile);
    }
}

bool JarClassFileReader::open(string& jarFilePath, string& classFileName) {
    int error = 0;
    mJarFile= zip_open(jarFilePath.c_str(), ZIP_RDONLY, &error);
	if (mJarFile == nullptr) {
		cout << "zip_open failed, err:" << error << ", details:" << strerror(errno) << endl;
		goto failed;
	}

    mClassFileInJar = zip_fopen(mJarFile, classFileName.c_str(), 0);
    if (mClassFileInJar == nullptr) {
        cout << "zip_fopen "<< classFileName <<" failed failed, error details:" << strerror(errno) << endl;
		goto failed;
    }
    return true;
failed:
    close();
    return false;
}

void JarClassFileReader::close() {
    if (mClassFileInJar != nullptr) {
        zip_fclose(mClassFileInJar);
        mClassFileInJar = nullptr;
    }
    if (mJarFile != nullptr) {
        zip_close(mJarFile);
        mJarFile = nullptr;
    }
}

size_t JarClassFileReader::readFromFile(void *buffer, size_t size) {
    size_t readBytes = zip_fread(mClassFileInJar, buffer, size);
    return readBytes;
}

}