#include <stdio.h>
#include "MyVmConfig.h"
#include "ClassLoader.h"
#include "classloader/ConstantInfo.h"

using namespace myvm;

int main(const char* arg[]) {
	printf("hello world, this is a vm toy!\n");
	testClassLoader();

	ConstantPackage constantPackage;
	constantPackage.nameIndex = 100;

	return 0;
}