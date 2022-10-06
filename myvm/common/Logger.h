#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <memory>
#include <vector>
#include <list>
#include <string>
#include <mutex>
using namespace std;

#include "FileWriter.h"
using namespace myvm;

namespace myvm {

#define LOGI(log) \
    Logger::getInstance()->i(log)

#define LOGV(log) \
    Logger::getInstance()->v(log)

enum LogLevel {
    VERBOSE,
    INFO,
};

const char* logLevelString(uint8_t level);

class LogItem {
public:
    LogItem(uint8_t level, const char* message);
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
    uint8_t level;
    string formatLog;
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
    void v(const char* message);
    void i(const char* message);
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