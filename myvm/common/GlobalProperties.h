#ifndef _GLOBAL_PROPERTIES_H_
#define _GLOBAL_PROPERTIES_H_

#include <memory>
#include <map>
#include <string>
using namespace std;

namespace myvm {

class GlobalProperties {
public:
    GlobalProperties();
    virtual ~GlobalProperties();
    string& getProperty(const string& name);
    bool containsProperty(const string& name);
    void addProperty(const string& name, const string& value);
    static shared_ptr<GlobalProperties> getInstance();
private:
    map<string, string> mProperties;
    static shared_ptr<GlobalProperties> sInstance;
    static string sEmptyString;
};

}

#endif
