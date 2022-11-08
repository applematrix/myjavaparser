#ifndef _BOOTSTRAP_CLASS_LOADER_H_
#define _BOOTSTRAP_CLASS_LOADER_H_

#include <map>
#include <vector>
#include <list>
#include <string>
#include "ClassInfo.h"
#include "ClassLoader.h"
using namespace std;
using namespace myvm;

namespace myvm {

class BootstrapClassLoader : public ClassLoader {
public:
    static shared_ptr<BootstrapClassLoader> getInstance();
    virtual ~BootstrapClassLoader();
    virtual shared_ptr<ClassInfo> loadClass(string& className);
    virtual shared_ptr<ClassInfo> loadClassFromFile(string& classFile);
    virtual shared_ptr<ClassInfo> loadClassFromJar(string& jarFile, string& className, string& fileName);
    shared_ptr<ClassInfo> loadClassFromBootclassPathJar(string& className);
    bool loadClassFromClassPath(string& className);
    virtual shared_ptr<ClassInfo> getClassByName(const string& name);
    virtual shared_ptr<ClassInfo> getClassByName(const char* name);
    void addClass(string& name, shared_ptr<ClassInfo> clazz);
private:
    BootstrapClassLoader();
    bool classLoaded(string& name);
    void covertPath(string& path);
private:
    static shared_ptr<BootstrapClassLoader> sInstance;
    map<string, shared_ptr<ClassInfo>> mLoadedClasses;
    list<string> mPendingLoadClasses;
    vector<string> mBootClassPathes;
    vector<string> mBootClassJars;
};

}
#endif
