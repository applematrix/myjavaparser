#include "GlobalProperties.h"

namespace myvm {

shared_ptr<GlobalProperties> GlobalProperties::sInstance = nullptr;

GlobalProperties::GlobalProperties() {
}

GlobalProperties::~GlobalProperties() {
}

string &GlobalProperties::getProperty(const string &name)
{
    if (mProperties.find(name) != mProperties.end()) {
        return mProperties[name];
    }
    return string("");
}

shared_ptr<GlobalProperties> GlobalProperties::getInstance()
{
    if (sInstance == nullptr) {
        sInstance = make_shared<GlobalProperties>();
    }
    return sInstance;
}

void GlobalProperties::addProperty(const string& name, const string& value) {
    mProperties[name] = value;
}

}