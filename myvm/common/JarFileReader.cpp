#include "JarFileReader.h"
#include <iostream>
#include <errno.h>
#include <string.h>
using namespace std;
using namespace myvm;

const char* MANIFEST = "META-INF/MANIFEST.MF";

namespace myvm {

JarFileReader::JarFileReader() {
    mJarFile = nullptr;
    mClassFileInJar = nullptr;
}

JarFileReader::~JarFileReader() {
    if (mJarFile != nullptr) {
        zip_close(mJarFile);
    }
}

void JarFileReader::open(const char* jarFilePath, const char* classFileName) {
    int error = 0;
    mJarFile= zip_open(jarFilePath, ZIP_RDONLY, &error);
	if (mJarFile == nullptr) {
		cout << "zip_open failed, err:" << error << ", details:" << strerror(errno) << endl;
		goto failed;
	}

    mClassFileInJar = zip_fopen(mJarFile, MANIFEST, 0);
    if (mClassFileInJar == nullptr) {
        cout << "zip_fopen "<< classFileName <<" failed failed, error details:" << strerror(errno) << endl;
		goto failed;
    }
    return;
failed:
    close();
}

void JarFileReader::close() {
    if (mClassFileInJar != nullptr) {
        zip_fclose(mClassFileInJar);
        mClassFileInJar = nullptr;
    }
    if (mJarFile != nullptr) {
        zip_close(mJarFile);
        mJarFile = nullptr;
    }
}

}