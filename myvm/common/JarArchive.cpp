#undef LOG_TAG
#define LOG_TAG "JarArchive"
#include "../common/Logger.h"

#include "JarArchive.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>
using namespace std;

const static char* MANIFEST = "META-INF/MANIFEST.MF";
const static int32_t BUFFER_SIZE = 1024;

namespace myvm {

static string trim(const string& content) {
    int startPos = content.find_first_not_of(' ');
    if (startPos == string::npos) startPos == 0;
    int endPos = content.find_last_not_of(' ');
    if (endPos == string::npos) startPos == content.size();
    return content.substr(startPos, endPos - startPos + 1);
}

/// ////////////////////////////////////////////////////////////////////
Manifest::Manifest() {
}

Manifest::~Manifest() {
}

void Manifest::loadManifestFromJar(zip_t *jar) {
    zip_file_t * file = zip_fopen(jar, MANIFEST, 0);
    if (file == nullptr) {
        LOGW("zip_fopen manifest failed failed, error details: %s", strerror(errno));
		return;
    }

    char buffer[BUFFER_SIZE + 1] = {0};
    const char* startPos = nullptr;
    bool pullData = true;
    string line;
    while(true) {
        int readSize = 0;
        if (pullData) {
            memset(buffer, 0, sizeof(buffer));
            readSize = zip_fread(file, buffer, BUFFER_SIZE);
            if (readSize == 0) {
                //LOGW("no more content in the file");
                break;
            } else if (readSize == -1) {
                //LOGW("zip_fopen failed, err:");
                break;
            }
            startPos = buffer;
        }
        const char *endPos;
        int8_t tokenSize = 0;
        if ((endPos = strstr(startPos, "\r\n")) != nullptr) {
            tokenSize = 2;
        }
        else if ((endPos = strstr(startPos, "\n")) != nullptr) {
            tokenSize = 1;
        }

        if (endPos != nullptr) {
            line.append(startPos, (endPos - startPos));
            string key, value;
            if (split(line, key, value))
            {
                mAttributes[key] = value;
            }
            line.resize(0);
            pullData = false;
            startPos = endPos + tokenSize;
        } else{
            line.append(startPos);
            pullData = true;
        }
    }
    if (!line.empty()) {
        string key, value;
        if (split(line, key, value)) {
            mAttributes[key] = value;
        }
    }
    return;
}

bool Manifest::split(string &attribute, string& key, string& value) {
    auto pos = attribute.find(":");
    if (pos == string::npos) {
        return false;
    }
    key = trim(attribute.substr(0, pos));
    value = trim(attribute.substr(pos+1));
    return true;
}

string Manifest::getMainClass() const {
    auto iter = mAttributes.find("MainClass");
    if (iter != mAttributes.end()) {
        return iter->first;
    }
    return string("");
}

////////////////////////////////////
JarArchive::JarArchive() {
}

JarArchive::~JarArchive() {
    if (mOpenedJar != nullptr) {
        zip_close(mOpenedJar);
        mOpenedJar = nullptr;
    }
}

void JarArchive::loadFile(string& path) {
    mJarPath = path;
    int error = 0;
    mOpenedJar= zip_open(path.c_str(), ZIP_RDONLY, &error);
    if (mOpenedJar == nullptr) {
		LOGW("zip_open failed, err:%d, details: %s", error, strerror(errno));
        return;
	}

    int filesInJar = zip_get_num_files(mOpenedJar);
	for (int i = 0; i < filesInJar; i++) {
		const char* fileName = zip_get_name(mOpenedJar, i, ZIP_FL_ENC_RAW);

        if (fileName != nullptr && fileName[strlen(fileName)-1] == '/') {
            //LOGW("%s is a directory", fileName);
            continue;
        }
        mFileList.push_back(fileName);
	}
    
    mManifest = make_shared<Manifest>();
    mManifest->loadManifestFromJar(mOpenedJar);
}

shared_ptr<Manifest> JarArchive::getManifest() const {
    return mManifest;
}

string JarArchive::getMainClass() const {
    auto manifest = getManifest();
    if (manifest == nullptr) {
        LOGW("No manifest in the jar!");
    }
    return manifest->getMainClass();
}

bool JarArchive::containsClass(string& className) {
    for (auto file : mFileList) {
        if (file.compare(className) == 0) {
            return true;
        }
    }
    return false;
}

}