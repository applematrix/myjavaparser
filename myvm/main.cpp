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
#include "common/utils.h"
#include "classloader/ThreadLocalStorage.h"
#include "classloader/Frame.h"
#include "classloader/CodeAttr.h"

#include <thread>
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

	Method *mainMethod = mClasssFile->findMainMethod();
	if (mainMethod == nullptr) {
		cout << "No main entry method in the class" << endl;
	} else {
		std::thread mainThread([](Method *method) {
			ThreadLocalStorage::getInstance()->intialize();
			CodeAttr* codeAttr = method->getCodeAttr();

			shared_ptr<Frame> frame = make_shared<Frame>(method, codeAttr->maxLocals, 0);
			cout << "Invoke the main method!" << endl;
			cout << INDENTS[0] << "{" << endl;
			method->invoke(frame);
			cout << INDENTS[0] << "}" << endl;
			}, mainMethod);

		mainThread.join();
	}
	delete mClasssFile;

	return 0;
}