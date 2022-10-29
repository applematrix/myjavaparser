/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */
#include <iostream>
#include <cstring>
#include "ConstantInfo.h"
#include "ClassInfo.h"

using namespace std;

namespace myvm {

const char* ConstantInfo::typeString() {
    switch (tag) {
        case CONSTANT_UTF8:	return "CONSTANT_UTF8";
        case CONSTANT_INTEGER: return "CONSTANT_INTEGER";
        case CONSTANT_FLOAT: return "CONSTANT_FLOAT";
        case CONSTANT_LONG:	return "CONSTANT_LONG";
        case CONSTANT_DOUBLE: return "CONSTANT_DOUBLE";
        case CONSTANT_CLASS: return "CONSTANT_CLASS";
        case CONSTANT_STRING: return "CONSTANT_STRING";
        case CONSTANT_FIELDREF:	return "CONSTANT_FIELDREF";
        case CONSTANT_METHODREF: return "CONSTANT_METHODREF";
        case CONSTANT_INTERFACE_METHODREF:  return "CONSTANT_INTERFACE_METHODREF";
        case CONSTANT_NAMEANDTYPE: return "CONSTANT_NAMEANDTYPE";
        case CONSTANT_METHODHANDLE:	return "CONSTANT_METHODHANDLE";
        case CONSTANT_METHODTYPE: return "CONSTANT_METHODTYPE";
        case CONSTANT_DYNAMIC: return "CONSTANT_DYNAMIC";
        case CONSTANT_INVOKEDYNAMIC: return "CONSTANT_INVOKEDYNAMIC";
        case CONSTANT_CONSTANT_MODULE: return "CONSTANT_CONSTANT_MODULE";
        default: return "unknown tag";
    }
}

void ConstantClass::dump(const ClassInfo* classInfo) {
    //cout << "ConstantClass:" << classInfo->getUtf8ConstantName(nameIndex) << endl;
}

bool ConstantUtf8::equals(string& str) const {
    return length == str.length() && strncmp((const char*)bytes, str.c_str(), length) == 0;
}

}
