#ifndef _THREAD_LOCAL_STORAGE_H_
#define _THREAD_LOCAL_STORAGE_H_

#include "OperandStack.h"

#include <memory>
using namespace std;
using namespace myvm;

namespace myvm {

class ThreadLocalStorage {
public:
    ThreadLocalStorage();
    ~ThreadLocalStorage();
    void intialize();
    shared_ptr<OperandStack> getStack();
    static ThreadLocalStorage* getInstance();
private: 
    static ThreadLocalStorage* sInstance;
    shared_ptr<OperandStack> mStack;
};

}


#endif