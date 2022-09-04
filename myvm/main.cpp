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
#include "windows/Environment.h"
#include "classloader/BootstrapClassLoader.h"

using namespace myvm;
using namespace std;

int main(int argc, const char* args[]) {
	cout << "hello world, this is a vm toy!" << endl;
	if (argc <= 1) {
		cout << "no class file specified" << endl;
		return -1;
	}
	const char* path = args[1];

	BootstrapClassLoader *mBootstrapClassLoder = BootstrapClassLoader::getInstance();
	mBootstrapClassLoder->loadClassFromFile(path);

	ClassFileInfo* mClasssFile = new ClassFileInfo();
	mClasssFile->loadFromFile(path);

	//wstring javaHomePath = getJavaHomePath();
	testWin();

	Method *mainMethod = mClasssFile->findMainMethod();
	if (mainMethod == nullptr) {
		cout << "No main entry method in the class" << endl;
	} else {
		cout << "Invoke the main method!" << endl;
		mainMethod->invoke(mClasssFile);
	}


	delete mClasssFile;

	return 0;
}