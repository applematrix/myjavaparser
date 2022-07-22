#ifndef _ATTRIBUTE_FACTORY_H_
#define _ATTRIBUTE_FACTORY_H_
#include "AttributeInfo.h"
#include "FileReader.h"
#include "ClassFileInfo.h"

namespace myvm {

class AttributeFactory {
public:
    static AttributeInfo* loadFromFile(ClassFileInfo* classInfo, FileReader* fileReader);
    static AttributeInfo* loadFromFile(FileReader* fileReader) {/*TODO*/};
};

}
#endif