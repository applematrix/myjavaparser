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
    ClassInfo* loadClassFromFile(string& classFile);
    ClassInfo* loadClassFromBootclassPathJar(string& className);
    ClassInfo* loadClassFromJar(string& jarFile, string& className, string& fileName);
    bool loadClassFromClassPath(string& className);
    ClassInfo* getClassByName(const string& name);
    ClassInfo* getClassByName(const char* name);
    void addClass(string& name, ClassInfo *clazz);
private:
    BootstrapClassLoader();
    bool classLoaded(string& name);
    void covertPath(string& path);
private:
    static BootstrapClassLoader *sInstance;
    map<string, ClassInfo*> mLoadedClasses;
    vector<string> mBootClassPathes;
    vector<string> mBootClassJars;
};

}
#endif
