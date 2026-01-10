#include "TSLogger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

void TSLogger::PrintMessages()
{
    LogMessage message;
    mLogQueue.waitAndPop(message);

    if (auto LogEmitter = message.mLogEmitter.lock())
    {
        std::ostringstream ossTime;
        if (LogEmitter->mWriteTime)
        {
            std::time_t t = std::time(nullptr); 
            std::tm tm_safe; 
            localtime_s(&tm_safe, &t);
            ossTime << std::put_time(&tm_safe, "%H:%M:%S") << " - ";
        }        
        std::cout << "[" << ossTime.str() << LogEmitter->mName << "]" << LogEmitter->mColor.mColorCode << message.mText << Color::RESET.mColorCode << "\n";
    }
    else
    {
        std::cout << message.mColor.mColorCode << message.mText << Color::RESET.mColorCode << "\n";
    }
}

void TSLogger::PushLogMessage(LogMessage message)
{
    mLogQueue.push(std::move(message));
}
