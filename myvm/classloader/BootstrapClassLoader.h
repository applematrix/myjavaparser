#ifndef _BOOTSTRAP_CLASS_LOADER_H_
#define _BOOTSTRAP_CLASS_LOADER_H_

#include <map>
#include <string>
#include "ClassInfo.h"
using namespace std;
using namespace myvm;

namespace myvm {

class BootstrapClassLoader {
public:
    static BootstrapClassLoader *getInstance();
    ~BootstrapClassLoader();
    shared_ptr<ClassInfo> loadClassFromFile(string& classFile);
    shared_ptr<ClassInfo> loadClassFromBootclassPathJar(string& className);
    shared_ptr<ClassInfo> loadClassFromJar(string& jarFile, string& className, string& fileName);
    bool loadClassFromClassPath(string& className);
    shared_ptr<ClassInfo> getClassByName(const string& name);
    shared_ptr<ClassInfo> getClassByName(const char* name);
    void addClass(string& name, shared_ptr<ClassInfo> clazz);
private:
    BootstrapClassLoader();
    bool classLoaded(string& name);
    void covertPath(string& path);
private:
    static BootstrapClassLoader *sInstance;
    map<string, shared_ptr<ClassInfo>> mLoadedClasses;
    vector<string> mBootClassPathes;
    vector<string> mBootClassJars;
};

}
#endif
