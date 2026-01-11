#pragma once

#include "TSQueue.h"
#include "ILogger.h"


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
