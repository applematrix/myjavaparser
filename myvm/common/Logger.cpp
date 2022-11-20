#include "Logger.h"
#include <thread>
#include <memory>

#define LOG_STD_OUT

using namespace myvm;
using namespace std;

const uint32_t MAX_CACHED_LOGS = 200;
const uint32_t CACHED_LOG_AGE = 10; //seconds
const uint8_t LOGGER_POOL_SIZE = 10;

namespace myvm {

const char* logLevelString(uint8_t level) {
    switch (level) {
    case VERBOSE:
        return "V";
    case DEBUG:
        return "D";
    case INFO:
        return "I";
    case WARNING:
        return "V";
    case ERROR:
        return "E";
    default:
        return "U";
    }
}

LogItem::LogItem(uint8_t _level, const char* _tag, const char* message)
    : level(_level) {
    auto now = chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
    tag = _tag;
    std::thread::id tid = this_thread::get_id();
    threadId = *((uint64_t*)&tid);

    format(message);
}

LogItem::~LogItem() {
}

void LogItem::format(const char* message) {
    struct tm* localTime = localtime((time_t*)&timestamp);
    const uint32_t BUFFER_LEN = 2048;
    char buffer[BUFFER_LEN] = { 0 };
    uint32_t year = localTime->tm_year + 1900;
    uint32_t month = localTime->tm_mon + 1;
    uint32_t day = localTime->tm_mday;
    uint32_t hour = localTime->tm_hour;
    uint32_t min = localTime->tm_min;
    uint32_t sec = localTime->tm_sec;

    uint32_t len = snprintf(buffer, BUFFER_LEN, "%d-%02d-%02d %02d:%02d:%02d %20ld ", year, month, day, hour, min, sec, threadId);
    snprintf(buffer + len, BUFFER_LEN - len, "%s %-10s: %s", logLevelString(level), tag.c_str(), message);
    formatedLog = buffer;
}

/////////////////////////////////////////////
CachedLog::CachedLog() {

}

CachedLog::~CachedLog() {

}

void CachedLog::addLog(shared_ptr<LogItem> log) {
    std::unique_lock<std::mutex> lock(mutex);
    logs.push_back(log);
}

shared_ptr<LogItem> CachedLog::popFront() {
    std::unique_lock<std::mutex> lock(mutex);
    shared_ptr<LogItem> item = logs.front();
    logs.pop_front();
    return item;
}

/////////////////////////////////////////////
Logger* Logger::sInstance = nullptr;
std::mutex sSingletonMutex;

/*static */Logger* Logger::getInstance() {
    std::unique_lock<std::mutex> lock(sSingletonMutex);
    return sInstance;
}

/*static */void Logger::initialize() {
    std::unique_lock<std::mutex> lock(sSingletonMutex);
    if (sInstance == nullptr) {
        sInstance = new Logger();
    }
}

/*static */void Logger::clear() {
    std::unique_lock<std::mutex> lock(sSingletonMutex);
    if (sInstance != nullptr) {
        delete sInstance;
        sInstance = nullptr;
    }
}

Logger::Logger() {
#ifdef LOG_STD_OUT
    mFileWriter = make_shared<FileWriter>();
#else
    auto now = chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
	struct tm* localTime = localtime(&t);
    char buffer[1024];
    uint32_t year = localTime->tm_year + 1900;
    uint32_t month = localTime->tm_mon + 1;
    uint32_t day = localTime->tm_mday;
    uint32_t hour = localTime->tm_hour;
    uint32_t min = localTime->tm_min;

    snprintf(buffer, 1024, "%d-%02d-%02d-%02d-%02d.log", year, month, day, hour, min);
    mFileWriter = make_shared<FileWriter>(buffer);
#endif
    mLastFlushTime = chrono::system_clock::now();
}

Logger::~Logger() {
    flush();
}

void Logger::v(const char* tag, const char* message) {
    shared_ptr<LogItem> log = make_shared<LogItem>(VERBOSE, tag, message);
    writeLog(log);
}

void Logger::d(const char* tag, const char* message) {
    shared_ptr<LogItem> log = make_shared<LogItem>(DEBUG, tag, message);
    writeLog(log);
}

void Logger::i(const char* tag, const char* message) {
    shared_ptr<LogItem> log = make_shared<LogItem>(INFO, tag, message);
    writeLog(log);
}

void Logger::w(const char* tag, const char* message) {
    shared_ptr<LogItem> log = make_shared<LogItem>(WARNING, tag, message);
    writeLog(log);
}

void Logger::e(const char* tag, const char* message) {
    shared_ptr<LogItem> log = make_shared<LogItem>(ERROR, tag, message);
    writeLog(log);
}

void Logger::writeLog(shared_ptr<LogItem> log) {
    uint8_t index = log->threadId % LOGGER_POOL_SIZE;

    shared_ptr<CachedLog> cacheLogger = nullptr;
    bool needFlush = false;
    bool force = true; //TODO:
    {
        std::unique_lock<std::mutex> lock(mPrimaryMutex);
        
        while (mCachedLogs.size() <= index) {
            mCachedLogs.push_back(make_shared<CachedLog>());
        }
        cacheLogger = mCachedLogs.at(index);
        mCachedLogItems++;
        needFlush |= mCachedLogItems > MAX_CACHED_LOGS;
        auto duration = chrono::system_clock::now() - mLastFlushTime;
        if (duration > chrono::seconds(CACHED_LOG_AGE) && mCachedLogItems > 1
            || mCachedLogItems > MAX_CACHED_LOGS) {
            needFlush = true;
        }
    }
    cacheLogger->addLog(log);
    if (needFlush || force) {
        flush();
    }
}

void Logger::flush() {
    vector<shared_ptr<LogItem>> mergedLogs;
    {
        std::unique_lock<std::mutex> lock(mPrimaryMutex);
        mLastFlushTime = chrono::system_clock::now();
        mergeLog(mergedLogs);
    }
    for (auto log : mergedLogs) {
        mFileWriter->writeMessage(log->formatedLog);
    }
}

void Logger::mergeLog(vector<shared_ptr<LogItem>>& mergedLogs) {
    bool hasLog = true;
    while (hasLog) {
        shared_ptr<LogItem> oldestLog = nullptr;
        hasLog = false;
        for (int i = 0; i < mCachedLogs.size(); i++)
        {
            shared_ptr<CachedLog> pooledLogger = mCachedLogs.at(i);
            if (pooledLogger->size() == 0) {
                continue;
            }
            shared_ptr<LogItem> log = pooledLogger->popFront();
            if (oldestLog == nullptr || *oldestLog > *log.get())
            {
                oldestLog = log;
            }
            hasLog = true;
        }
        if (oldestLog != nullptr) {
            mergedLogs.push_back(oldestLog);
        }
    }
}

}