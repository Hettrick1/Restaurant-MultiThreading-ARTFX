#pragma once

#include "TSQueue.h"
#include "ILogger.h"

// class that print in the console logs in a thread safe way
// avoid multiple threads to write at the same time in the console
// handles colors, who sent the message and time
// the console prints are managed in a separated thread
class TSLogger : public ILogger
{
private:
    TSQueue<LogMessage> mLogQueue;
    std::thread mLogThread;
    void LoggerThread();
    
public:
    ~TSLogger() override = default;
    void StartLogging();
    bool PrintMessagesOrShouldStop();
    void PushLogMessage(LogMessage message) override;
    bool HasMessagesToPrint();
    void StopLogging();
};
