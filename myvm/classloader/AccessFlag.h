#ifndef _ACCESS_FLAG_H_
#define _ACCESS_FLAG_H_

/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include <stdint.h>
#include <string>
using namespace std;

namespace myvm {

enum AccessFlag {
    ACC_PUBLIC = 0x0001,
    ACC_FINAL = 0x0010,
    ACC_SUPER = 0x0020,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0200,
    ACC_SYNTHETIC = 0x1000,
    ACC_ANNOTATION = 0x2000,
    ACC_ENUM = 0x4000,
    ACC_MODULE = 0x8000,
};

void accessFlagToString(uint16_t flag, string& outString);

}

#endif