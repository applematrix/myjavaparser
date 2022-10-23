#include "JarClassFileReader.h"
#include <iostream>
#include <errno.h>
#include <string.h>
using namespace std;
using namespace myvm;

const char* MANIFEST = "META-INF/MANIFEST.MF";

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

void JarClassFileReader::open(const char* jarFilePath, const char* classFileName) {
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

}