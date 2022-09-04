#include "BootstrapClassLoader.h"
#include "ClassFileInfo.h"
#include "types.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include  <io.h>
#endif

using namespace std;

#ifdef WIN32
// test code
#define BOOT_CLASS_PATH "C:\\Users\\huangdezhi\\Documents\\GitHub\\myjavaparser\\myvm\\test\\jdk_classes"
#else
#define BOOT_CLASS_PATH ""
#endif

namespace myvm {

BootstrapClassLoader * BootstrapClassLoader::sInstance = nullptr;

BootstrapClassLoader::BootstrapClassLoader() {
    mBootClassPathes.push_back(BOOT_CLASS_PATH);
}

BootstrapClassLoader::~BootstrapClassLoader() {
}

/*static*/ BootstrapClassLoader* BootstrapClassLoader::getInstance() {
    // TODO: multi-thread access
    if (sInstance == nullptr) {
        sInstance = new BootstrapClassLoader();
    }
    return sInstance;
}

ClassFileInfo* BootstrapClassLoader::getClassByName(string& name) {
    return mLoadedClasses[name];
}

void BootstrapClassLoader::addClass(string& name, ClassFileInfo *clazz) {
    // TODO: multi-thread access
    if (classLoaded(name)) {
        cout << name << " already add into bootstrap classloader" << endl;
        return;
    }
    mLoadedClasses[name] = clazz;
}

bool BootstrapClassLoader::loadClassFromFile(const char* path) {
    ClassFileInfo *clazz = new ClassFileInfo();
	clazz->loadFromFile(path);

    std::string className = clazz->getClassName();
    addClass(className, clazz);
    return true;
}

bool BootstrapClassLoader::loadClassFromFile(string& classFile) {
    return loadClassFromFile(classFile.c_str());
}

bool BootstrapClassLoader::loadClassFromJar(string& jarFile, string className) {
    return false;
}

bool BootstrapClassLoader::loadClassFromClassPath(string& className) {
    if (classLoaded(className)) {
        return true;
    }
    for (auto classPath : mBootClassPathes) {
        auto fullPath = classPath + FILE_SPERATOR + className;
        covertPath(fullPath);
#ifdef WIN32
        if (_access(fullPath.c_str(), 0) != 0)
#else
        // TODO:
        if (access(fullPath.c_str(), 0) != 0)
#endif
        {
            cout << fullPath << " not found!" << endl;
        }

        if(loadClassFromFile(fullPath)) {
            cout << "Loaded class:" << className << " from " << fullPath << endl;
            return true;
        }
    }
    cout << "Can't found the class:" << className << endl;
    return false;
}

bool BootstrapClassLoader::classLoaded(string& name) {
    return mLoadedClasses.find(name) != mLoadedClasses.end();
}

void BootstrapClassLoader::covertPath(string& path) {
    size_t pos = 0;
#ifdef WIN32
    while ((pos = path.find('/', pos)) != string::npos) {
        path.replace(pos, 1, "\\");
    }
#endif
    path.append(CLASS_SUFFIX);
    return;
}

}