#ifndef _JAR_ARCHIVE_H_
#define _JAR_ARCHIVE_H_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <zip.h>
using namespace std;

namespace myvm {

class Manifest {
public:
    Manifest();
    virtual ~Manifest();
    void loadManifestFromJar(zip_t *jar);
    string getMainClass() const;
private:
    bool split(string &attribute, string& key, string& value);

private:
    string mVersion;
    string mCreatedBy;
    string mSignatureVersion;
    const string mMainClass;// for executable jar

    map<string, string> mAttributes;
};

class JarArchive {
public:
    JarArchive();
    virtual ~JarArchive();
    void loadFile(string& path);
    string getMainClass() const ;
    shared_ptr<Manifest> getManifest() const;
    bool containsClass(string& className);
private:
    string mJarPath;
    zip_t *mOpenedJar = nullptr;
    shared_ptr<Manifest> mManifest;
    vector<string> mFileList;
};


}


#endif