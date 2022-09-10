#include "ThreadLocalStorage.h"
#include <thread>

using namespace std;
using namespace myvm;

namespace myvm {

//thread_local static unique_ptr<ThreadLocalStorage> sInstance = nullptr;

ThreadLocalStorage* ThreadLocalStorage::sInstance = nullptr;

ThreadLocalStorage::ThreadLocalStorage() {
}

ThreadLocalStorage::~ThreadLocalStorage() {
}

ThreadLocalStorage* ThreadLocalStorage::getInstance() {
    if (sInstance == nullptr) {
        sInstance = new ThreadLocalStorage();
    }
    return sInstance;
}

void ThreadLocalStorage::intialize() {
    mStack = make_shared<OperandStack>();
}

shared_ptr<OperandStack> ThreadLocalStorage::getStack() {
    return mStack;
}

}