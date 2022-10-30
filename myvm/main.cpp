/*
 * Copyright (c) 2022 Huang Dezhi <cshdzxjtu@163.com>
 * All Rights Reserved
 *
 */

#include <stdio.h>
#include <iostream>
#include "MyVmConfig.h"
#include "classloader/ClassLoader.h"
#include "classloader/ConstantInfo.h"
#include "classloader/ClassInfo.h"
#include "windows/Environment.h"
#include "classloader/BootstrapClassLoader.h"
#include "common/utils.h"
#include "classloader/ThreadLocalStorage.h"
#include "classloader/Frame.h"
#include "classloader/CodeAttr.h"
#include "common/GlobalProperties.h"
#include "common/Logger.h"
#include "common/JarClassFileReader.h"
#include "common/JarArchive.h"

#include <cstring>
#include <thread>
#include <zip.h>
#include <errno.h>
using namespace myvm;
using namespace std;

#ifdef BUILD_DEBUG
#endif

static void parseArgs(int argc, const char* args[]) {
	string key = "";
	shared_ptr<GlobalProperties> globalProperty = GlobalProperties::getInstance();
	for (int i = 1; i < argc; i++) {
		string curArg = string(args[i]);
		if (!key.empty()) {
			globalProperty->addProperty(key, curArg);
			key = "";
			continue;
		}
		if (strcmp(args[i], "-classpath") == 0
			|| strcmp(args[i], "-cp") == 0) {
			key = "classpath";
		} else if (strcmp(args[i], "-jar") == 0) {
		}else {
			key = "jar";
			globalProperty->addProperty(key, curArg);
		}
	}

	// debug code
	if (!globalProperty->containsProperty("jar")
		&& !globalProperty->containsProperty("classpath")) {
		globalProperty->addProperty("classpath", "./test/testmyvm/target/classes/com/myvm/test/SimpleClass.class");
	}
}

int main(int argc, const char* args[]) {
	Logger::initialize();
	cout << "Hello world, I'm a toy vm, I can do simple jobs for you" << endl;

	parseArgs(argc, args);
	string mainClassName;
	shared_ptr<GlobalProperties> globalProperty = GlobalProperties::getInstance();
	
	BootstrapClassLoader *mBootstrapClassLoder = BootstrapClassLoader::getInstance();
	ClassInfo *mainClass = nullptr;
	if (globalProperty->containsProperty("classpath")) {
		mainClassName = globalProperty->getProperty("classpath");
		mainClass = mBootstrapClassLoder->loadClassFromFile(mainClassName);
		if (mainClass == nullptr) {
			cout << "load class from .class file failed" << endl;
			return -1;
		}
	} else if (globalProperty->containsProperty("jar")){
		string jarPath = globalProperty->getProperty("jar");
		shared_ptr<JarArchive> jarArchive = make_shared<JarArchive>();
		jarArchive->loadFile(jarPath);
		mainClassName = jarArchive->getMainClass();

		mainClass = mBootstrapClassLoder->loadClassFromBootclassPathJar(mainClassName);
		if (mainClass == nullptr) {
			cout << "load class from jar file failed" << endl;
			return -1;
		}
	} else {
		cout << "no class file specified" << endl;
		return -1;
	}

	LOGI("vm started");
	Method *mainMethod = mainClass->findMainMethod();
	if (mainMethod == nullptr) {
		cout << "No main entry method in the class" << endl;
	} else {
		std::thread mainThread([](Method *method) {
			ThreadLocalStorage::getInstance()->intialize();
			shared_ptr<CodeAttr> codeAttr = method->getCodeAttr();

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
