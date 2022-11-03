#include "BootstrapClassLoader.h"
#include "ClassInfo.h"
#include "utils.h"
#include "common/ClassFileReader.h"
#include "common/JarArchive.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include  <io.h>
#else
#include <unistd.h>
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
#ifndef WIN32
    mBootClassJars.push_back("/home/lenovo/github_upload/myjavaparser/myvm/test/jdk_classes/rt.jar");
#endif // !WIN32
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

shared_ptr<ClassInfo> BootstrapClassLoader::getClassByName(const string& name) {
    return mLoadedClasses.find(name) == mLoadedClasses.end() ? nullptr : mLoadedClasses[name];
}

shared_ptr<ClassInfo> BootstrapClassLoader::getClassByName(const char* name) {
    return getClassByName(string(name));
}

void BootstrapClassLoader::addClass(string& name, shared_ptr<ClassInfo> clazz) {
    // TODO: multi-thread access
    if (classLoaded(name)) {
        cout << name << " already add into bootstrap classloader" << endl;
        return;
    }
    cout << "Add class: " << name << " into bootstrap classloader" << endl;
    mLoadedClasses[name] = clazz;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFromFile(string& classFile) {
    shared_ptr<ClassInfo> clazz = make_shared<ClassInfo>();
    if (!clazz->loadFromFile(classFile)) {
        return nullptr;
    }

    std::string className = clazz->getClassName();
    addClass(className, clazz);
    return clazz;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFromBootclassPathJar(string& className) {
    string fullName = className;
    const char* suffix = ".class";
    if (fullName.length() > strlen(suffix)) {
        string tmp = fullName.substr(fullName.length() - strlen(suffix));
        if (tmp.compare(suffix) != 0) {
            fullName.append(".class");
        }
    }
    for (auto bootjar : mBootClassJars) {
        JarArchive jar;
        jar.loadFile(bootjar);
        if (jar.containsClass(fullName)) {
            return loadClassFromJar(bootjar, className, fullName);
        }
    }
    cout << className << " not found in all bootclass path jar" << endl;
    return nullptr;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFromJar(string& jarFile, string& className, string& fileName) {
    shared_ptr<ClassInfo> clazz = make_shared<ClassInfo>();
	if(!clazz->loadFromJar(jarFile, fileName)) {
        return nullptr;
    }
    addClass(className, clazz);
    return clazz;
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
