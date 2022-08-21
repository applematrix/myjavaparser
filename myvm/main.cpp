/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include <stdio.h>
#include <iostream>
#include "MyVmConfig.h"
#include "ClassLoader.h"
#include "classloader/ConstantInfo.h"
#include "ClassFileInfo.h"

using namespace myvm;
using namespace std;

int main(int argc, const char* args[]) {
	cout << "hello world, this is a vm toy!" << endl;
	
	if (argc <= 1) {
		cout << "no class file specified" << endl;
		return -1;
	}
	const char* path = args[1];
	ClassFileInfo* mClasssFile = new ClassFileInfo();
	mClasssFile->loadFromFile(path);

	mClasssFile->resolve();

	delete mClasssFile;

	return 0;
}