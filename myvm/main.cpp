/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include <stdio.h>
#include "MyVmConfig.h"
#include "ClassLoader.h"
#include "classloader/ConstantInfo.h"
#include "ClassFileInfo.h"

using namespace myvm;

int main(int argc, const char* args[]) {
	printf("hello world, this is a vm toy!\n");
	
	if (argc <= 1) {
		printf("no class file specified");
		return -1;
	}
	const char* path = args[1];
	ClassFileInfo* mClasssFile = new ClassFileInfo();
	mClasssFile->loadFromFile(path);
	delete mClasssFile;

	return 0;
}