#ifndef _UTILS_H_
#define _UTILS_H_
#include "stdint.h"
// typedef char int8_t;
// typedef unsigned char uint8_t;
// typedef short int int16_t;
// typedef unsigned short int uint16_t;
// typedef int int32_t;
// typedef unsigned int uint32_t;
// typedef long int64_t;
// typedef unsigned long uint64_t;

//typedef int32_t status_t;

#define CLASS_SUFFIX ".class"
#define OBJECT_CLASS "java/lang/Object"
#ifdef WIN32
#define FILE_SPERATOR '\\'
#else
#define FILE_SPERATOR '/'
#endif

const char* indent(uint16_t depth);
extern const char* INDENTS[];

#endif