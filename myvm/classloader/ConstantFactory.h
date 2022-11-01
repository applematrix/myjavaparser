/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CONSTANT_FACTORY_H_
#define _CONSTANT_FACTORY_H_
#include <memory>
#include "ConstantInfo.h"
#include "FileReader.h"

using namespace std;

namespace myvm {
class ConstantFactory {
public:
    static ConstantInfo* loadFromFile(shared_ptr<FileReader> fileReader);
};

}
#endif