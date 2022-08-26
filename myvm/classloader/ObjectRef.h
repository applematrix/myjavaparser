#ifndef _OBJECT_REF_H_
#define _OBJECT_REF_H_

#include <string>
using namespace std;

namespace myvm {

class ObjectRef {
public:
    ObjectRef() {}
    ObjectRef(const char* name);
    ~ObjectRef() {}

private:
    string mClassName;
};

}

#endif