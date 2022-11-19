#include "BootstrapClassLoader.h"
#include "ClassInfo.h"
#include "utils.h"
#include "../common/ClassFileReader.h"
#include "../common/JarArchive.h"
#include <algorithm>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include  <io.h>
#else
#include <unistd.h>
#endif

#undef LOG_TAG
#define LOG_TAG "BootstrapClassLoader"
#include "../common/Logger.h"

using namespace std;

#ifdef WIN32
// test code
#define BOOT_CLASS_PATH "C:\\Users\\huangdezhi\\Documents\\GitHub\\myjavaparser\\myvm\\test\\jdk_classes"
#else
#define BOOT_CLASS_PATH ""
#endif

namespace myvm {

shared_ptr<BootstrapClassLoader> BootstrapClassLoader::sInstance;

BootstrapClassLoader::BootstrapClassLoader() {
    mBootClassPathes.push_back(BOOT_CLASS_PATH);
#ifndef WIN32
    mBootClassJars.push_back("/home/lenovo/github_upload/myjavaparser/myvm/test/jdk_classes/rt.jar");
#endif // !WIN32
}

BootstrapClassLoader::~BootstrapClassLoader() {
}

/*static*/ shared_ptr<BootstrapClassLoader> BootstrapClassLoader::getInstance() {
    // TODO: multi-thread access
    if (sInstance == nullptr) {
        sInstance.reset(new BootstrapClassLoader());
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
        LOGW("%s already add into bootstrap classloader", name.c_str());
        return;
    }
    LOGI("Add class: %s into bootstrap classloader", name.c_str());
    mLoadedClasses[name] = clazz;
    drainPendingLoadClasses();
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFile(string& classFile) {
    LOGI("loadClassFromFile file: %s", classFile.c_str());
    shared_ptr<ClassInfo> clazz = make_shared<ClassInfo>(shared_from_this());
    if (!clazz->loadFromFile(classFile)) {
        return nullptr;
    }

    std::string className = clazz->getClassName();
    addClass(className, clazz);
    return clazz;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClass(string& className) {
    if (mLoadedClasses.find(className) != mLoadedClasses.end()) {
        return mLoadedClasses[className];
    }
    LOGI("loadClass class:%s", className.c_str());
    auto clazz = loadClassFromBootclassPathJar(className);
    return clazz;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFromBootclassPathJar(string& className) {
    LOGI("loadClassFromBootclassPathJar class:%s", className.c_str());
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
    LOGW("%s not found in all bootclass path jar", className.c_str());
    return nullptr;
}

shared_ptr<ClassInfo> BootstrapClassLoader::loadClassFromJar(string& jarFile, string& className, string& fileName) {
    shared_ptr<ClassInfo> clazz = make_shared<ClassInfo>(shared_from_this());
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

        if(loadClassFile(fullPath)) {
            LOGW("Loaded class: %s from %s", className.c_str(), fullPath.c_str());
            return true;
        }
    }
    LOGW("Can't found the class: %s", className.c_str());
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

void BootstrapClassLoader::addPendingLoadClass(string& className) {
    if (classLoaded(className)) {
        return;
    }
    if (find(mPendingLoadClasses.begin(), mPendingLoadClasses.end(), className) != mPendingLoadClasses.end()) {
        return;
    }
    mPendingLoadClasses.push_back(className);
}

void BootstrapClassLoader::drainPendingLoadClasses() {
    while (mPendingLoadClasses.size() > 0) {
        auto className = mPendingLoadClasses.front();
        mPendingLoadClasses.pop_front();
        LOGI("drainPendingLoadClasses load the class :%s ", className.c_str());
        loadClass(className);
    }
}

}
