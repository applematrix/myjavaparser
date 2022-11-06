#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <mutex>
#include <stdio.h>

#include "FileWriter.h"
#define MAX_LOG_LENGTH_PER_LINE 4096

using namespace myvm;
using namespace std;

namespace myvm {

#define LOG(...) \
    char log[MAX_LOG_LENGTH_PER_LINE] = {0}; \
    snprintf(log, MAX_LOG_LENGTH_PER_LINE, __VA_ARGS__);

#define LOGV(...) { \
    LOG(__VA_ARGS__); \
    Logger::getInstance()->v(LOG_TAG, log);\
}

#define LOGD(...) { \
    LOG(__VA_ARGS__);\
    Logger::getInstance()->d(LOG_TAG, log);\
}

#define LOGI(...) { \
    LOG(__VA_ARGS__);\
    Logger::getInstance()->i(LOG_TAG, log);\
}

#define LOGW(...) { \
    LOG(__VA_ARGS__);\
    Logger::getInstance()->w(LOG_TAG, log);\
}

#define LOGE(...) { \
    LOG(__VA_ARGS__);\
    Logger::getInstance()->w(LOG_TAG, log);\
}

enum LogLevel {
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
};

const char* logLevelString(uint8_t level);

class LogItem {
public:
    LogItem(uint8_t level, const char* tag, const char* message);
    ~LogItem();
    bool operator==(const LogItem& other) const {
        return timestamp == other.timestamp;
    }

    bool operator<(const LogItem& other) const {
        return timestamp < other.timestamp;
    }

    bool operator>(const LogItem& other) const {
        return timestamp > other.timestamp;
    }

    void format(const char* message);

    uint64_t timestamp;
    uint64_t threadId;
    string tag;
    uint8_t level;
    string formatedLog;
};

class CachedLog {
public:
    CachedLog();
    ~CachedLog();
    void addLog(shared_ptr<LogItem> log);
    shared_ptr<LogItem> popFront();
    uint32_t size() { return logs.size(); }
public:
    list<shared_ptr<LogItem>> logs;
    std::mutex mutex;
};

class Logger {
public:
    static Logger* getInstance();
    static void initialize();
    static void clear();
    virtual ~Logger();
    void v(const char* tag, const char* message);
    void d(const char* tag, const char* message);
    void i(const char* tag, const char* message);
    void w(const char* tag, const char* message);
    void e(const char* tag, const char* message);
    void writeLog(shared_ptr<LogItem> log);
    void flush();
private:
    Logger();
    void mergeLog(vector<shared_ptr<LogItem>>& mergedLogs);

    static Logger* sInstance;
    std::mutex mPrimaryMutex;
    
    vector<shared_ptr<CachedLog>> mCachedLogs;
    uint32_t mCachedLogItems = 0;
    std::chrono::system_clock::time_point mLastFlushTime;
    shared_ptr<FileWriter> mFileWriter;
};

}

#endif