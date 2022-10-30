#include "GlobalProperties.h"

namespace myvm {

shared_ptr<GlobalProperties> GlobalProperties::sInstance = nullptr;
string GlobalProperties::sEmptyString = "";

GlobalProperties::GlobalProperties() {
}

GlobalProperties::~GlobalProperties() {
}

string &GlobalProperties::getProperty(const string &name)
{
    if (mProperties.find(name) != mProperties.end()) {
        return mProperties[name];
    }
    return sEmptyString;
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

bool GlobalProperties::containsProperty(const string& name) {
    return mProperties.find(name) != mProperties.end();
}

}
