#ifndef _FILE_WRITER_H_
#define _FILE_WRITER_H_

#include <fstream>
using namespace std;

namespace myvm {

class FileWriter {
public:
    FileWriter();
    FileWriter(const char* path);
    virtual ~FileWriter();
    void writeMessage(const string& message);
private:
    ofstream mOut;
    bool useStdOut = false;
};

}

#endif