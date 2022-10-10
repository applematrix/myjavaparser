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
#include "common/GlobalProperties.h"
#include "common/Logger.h"

#include <cstring>
#include <thread>
using namespace myvm;
using namespace std;

static void parseArgs(int argc, const char* args[]) {
	string key = "";
	shared_ptr<GlobalProperties> globalProperty = GlobalProperties::getInstance();
	for (int i = 1; i < argc; i++) {
		string curArg = string(args[i]);
		if (!key.empty()) {
			globalProperty->addProperty(key, curArg);
			key = "";
		}
		if (strcmp(args[i], "-classpath") == 0
			|| strcmp(args[i], "-cp") == 0) {
			key = "classpath";
		} else {
			key = "mainclass";
			globalProperty->addProperty(key, curArg);
		}
	}
}

int main(int argc, const char* args[]) {
	Logger::initialize();
	cout << "hello world, this is a vm toy!" << endl;
	if (argc <= 1) {
		cout << "no class file specified" << endl;
		return -1;
	}
	const char* path = args[1];

	parseArgs(argc, args);
	string mainClass = GlobalProperties::getInstance()->getProperty("mainclass");
	if (mainClass.empty()) {
		cout << "No main class specified" << endl;
		return -1;
	}

	LOGI("vm started");

	BootstrapClassLoader *mBootstrapClassLoder = BootstrapClassLoader::getInstance();
	mBootstrapClassLoder->loadClassFromFile(mainClass);

	shared_ptr<ClassFileInfo> mClasssFile = make_shared<ClassFileInfo>();
	mClasssFile->loadFromFile(mainClass.c_str());

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
	LOGI("vm closed");

	Logger::clear();
	return 0;
}
