#include "TSLogger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

void TSLogger::StartLogging()
{
    mLogThread = std::thread(&TSLogger::LoggerThread, this);
}

bool TSLogger::PrintMessagesOrShouldStop()
{
    std::shared_ptr<LogMessage> message;
    message = mLogQueue.waitAndPop();

    if (!message)
    {
        std::cout << Color::WHITE.mColorCode << "STOP logging" << Color::RESET.mColorCode << std::endl;
        return true;
    }
    
    if (auto LogEmitter = message->mLogEmitter.lock())
    {
        std::ostringstream ossTime;
        if (LogEmitter->mWriteTime)
        {
            std::time_t t = std::time(nullptr); 
            std::tm tm_safe; 
            localtime_s(&tm_safe, &t);
            ossTime << std::put_time(&tm_safe, "%H:%M:%S") << " - ";
        }        
        std::cout << "[" << ossTime.str() << LogEmitter->mName << "]" << " " << LogEmitter->mColor.mColorCode << message->mText << Color::RESET.mColorCode << std::endl;
    }
    else
    {
        std::cout << message->mColor.mColorCode << message->mText << Color::RESET.mColorCode << std::endl;
    }
    return false;
}

void TSLogger::PushLogMessage(LogMessage message)
{
    mLogQueue.push(std::move(message));
}

bool TSLogger::HasMessagesToPrint()
{
    return !mLogQueue.empty();
}

void TSLogger::StopLogging()
{
    mLogQueue.close();
    mLogThread.join();
}

void TSLogger::LoggerThread()
{
    while (!PrintMessagesOrShouldStop());
}
