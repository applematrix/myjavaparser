/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _ATTRIBUTE_FACTORY_H_
#define _ATTRIBUTE_FACTORY_H_
//#include "AttributeInfo.h"
#include "FileReader.h"
#include "ClassInfo.h"

using namespace myvm;

namespace myvm {

struct AttributeInfo;
class ClassInfo;
class FileReader;

class AttributeFactory {
public:
    static AttributeInfo* loadFromFile(ClassInfo* classInfo, FileReader* fileReader);
};

}
#endif