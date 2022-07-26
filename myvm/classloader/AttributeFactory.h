#ifndef _ATTRIBUTE_FACTORY_H_
#define _ATTRIBUTE_FACTORY_H_
//#include "AttributeInfo.h"
#include "FileReader.h"
#include "ClassFileInfo.h"

using namespace myvm;

namespace myvm {

struct AttributeInfo;
class ClassFileInfo;
class FileReader;

class AttributeFactory {
public:
    static AttributeInfo* loadFromFile(ClassFileInfo* classInfo, FileReader* fileReader);
};

}
#endif