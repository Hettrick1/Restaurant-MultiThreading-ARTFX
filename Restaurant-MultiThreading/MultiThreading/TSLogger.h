#pragma once

#include "ConsoleColors.h"
#include "TSQueue.h"

#include <string>
#include <mutex>
#include <condition_variable>
#include <queue>

struct LogEmitter
{
    std::string mName;
    bool mWriteTime = false;
    Color mColor = Color::WHITE;

    LogEmitter(std::string name, bool writeTime = false, Color color = Color::WHITE)
    : mName{std::move(name)}, mWriteTime{writeTime},  mColor{std::move(color)}
    {
        
    }
};

struct LogMessage
{
    std::string mText = "Empty";
    Color mColor = Color::WHITE;
    std::weak_ptr<LogEmitter> mLogEmitter;

    LogMessage() = default;
    
    LogMessage(std::string text,  std::weak_ptr<LogEmitter> logEmitter)
        : mText(std::move(text)), mLogEmitter(std::move(logEmitter))
    {
        
    }
    LogMessage(std::string text,  Color color = Color::WHITE)
        : mText{std::move(text)}, mColor{std::move(color)}
    {
        
    }
};

class TSLogger
{
private:
    TSQueue<LogMessage> mLogQueue;
public:
    bool PrintMessagesOrShouldStop();
    void PushLogMessage(LogMessage message);
    bool HasMessagesToPrint();
    void StopLogging();
};
