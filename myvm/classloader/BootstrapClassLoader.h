#ifndef _BOOTSTRAP_CLASS_LOADER_H_
#define _BOOTSTRAP_CLASS_LOADER_H_

#include <map>
#include <string>
#include "ClassFileInfo.h"
using namespace std;
using namespace myvm;

namespace myvm {

class BootstrapClassLoader {
public:
    static BootstrapClassLoader *getInstance();
    ~BootstrapClassLoader();
    bool loadClassFromFile(const char *path);
    bool loadClassFromFile(string& classFile);
    bool loadClassFromJar(string& jarFile, string className);
    bool loadClassFromClassPath(string& className);
    ClassFileInfo* getClassByName(const string& name);
    ClassFileInfo* getClassByName(const char* name);
    void addClass(string& name, ClassFileInfo *clazz);
private:
    BootstrapClassLoader();
    bool classLoaded(string& name);
    void covertPath(string& path);
private:
    static BootstrapClassLoader *sInstance;
    map<string, ClassFileInfo*> mLoadedClasses;
    vector<string> mBootClassPathes;
};

}
#endif
