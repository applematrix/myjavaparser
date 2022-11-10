#ifndef _CLASS_LOADER_H_
#define _CLASS_LOADER_H_

#include <string>
#include <memory>

#include "ClassInfo.h"
using namespace std;
using namespace myvm;

namespace myvm {
class ClassInfo;

class ClassLoader: public enable_shared_from_this<ClassLoader> {
public:
    ClassLoader(){}
    virtual ~ClassLoader() {}
    virtual shared_ptr<ClassInfo>  loadClass(string& className) = 0;
    virtual shared_ptr<ClassInfo> loadClassFile(string& classFile) = 0;
    virtual shared_ptr<ClassInfo> loadClassFromJar(string& jarFile, string& className, string& fileName) = 0;
    virtual shared_ptr<ClassInfo> getClassByName(const string& name) = 0;
    virtual shared_ptr<ClassInfo> getClassByName(const char* name) = 0;
};

}
#endif