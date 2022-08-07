/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#ifndef _CONSTANT_FACTORY_H_
#define _CONSTANT_FACTORY_H_
#include "ConstantInfo.h"
#include "FileReader.h"

namespace myvm {
class ConstantFactory {
public:
    static ConstantInfo* loadFromFile(FileReader* fileReader);
};

}
#endif