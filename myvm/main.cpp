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
#include "classloader/ClassFileInfo.h"
#include "windows/Environment.h"
#include "classloader/BootstrapClassLoader.h"
#include "common/utils.h"
#include "classloader/ThreadLocalStorage.h"
#include "classloader/Frame.h"
#include "classloader/CodeAttr.h"
#include "common/GlobalProperties.h"
#include "common/Logger.h"
#include "common/JarFileReader.h"
#include "common/JarArchive.h"

#include <cstring>
#include <thread>
#include <zip.h>
#include <errno.h>
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
}

void readJarContent() {
	// Test
    int error;
	const char* testPath = "/home/lenovo/github_upload/myjavaparser/myvm/test/testmyvm/target/testmyvm-1.0-SNAPSHOT.jar";
    zip_t* zip = zip_open(testPath, ZIP_RDONLY, &error);
	if (zip == nullptr) {
		cout << "zip_open failed, err:" << error << ", details:" << strerror(errno) << endl;
		return;
	}

	int filesInJar = zip_get_num_files(zip);
	for (int i = 0; i < filesInJar; i++) {
		const char* fileName = zip_get_name(zip, i, ZIP_FL_ENC_RAW);
		cout << "zip file name:" << fileName << endl;
	}
	error = zip_close(zip);

}

void readZipContent() {
	int error;
	const char* testPath = "/home/lenovo/test_zip.zip";

	zip_t* zip = zip_open(testPath, ZIP_RDONLY, &error);
	if (zip == nullptr) {
		cout << "zip_open failed, err:" << error << ", details:" << strerror(errno) << endl;
		return;
	}

	int filesInJar = zip_get_num_files(zip);
	for (int i = 0; i < filesInJar; i++) {
		const char* fileName = zip_get_name(zip, i, ZIP_FL_ENC_RAW);
		cout << "zip file name:" << fileName << endl;

		zip_file_t * file = zip_fopen(zip, fileName, 0);
		if (file == nullptr) {
			cout << "open file is failed" << endl;
			return;
		}

		char buffer[1024] = {0};
		int size = zip_fread(file, buffer, 1024);
		if (size == -1) {
			cout << "read file failed" << endl;
		}
	}
}

int main(int argc, const char* args[]) {
	Logger::initialize();

	// test code
	readZipContent();
	readJarContent();

	cout << "Hello world, I'm a toy vm, I can do simple jobs for you" << endl;
	if (argc <= 1) {
		cout << "no class file specified" << endl;
		return -1;
	}
	const char* path = args[1];

	parseArgs(argc, args);
	string mainClass;
	string jarPath = GlobalProperties::getInstance()->getProperty("jar");
	if (jarPath.empty()) {
		mainClass = GlobalProperties::getInstance()->getProperty("classpath");
	} else {
		shared_ptr<JarArchive> jarArchive = make_shared<JarArchive>();
		jarArchive->loadFile(jarPath);
		mainClass = jarArchive->getMainClass();
	}

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
