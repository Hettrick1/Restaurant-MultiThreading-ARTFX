#include "Actor.h"

Actor::Actor(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger)
    : mName{std::move(name)}, mApplicationIsRunning{applicationIsRunning}, mLogEmitter{std::move(logEmitter)}, mLogger{std::move(logger)}
{
}

void Actor::StartThread()
{
    mActorThread = std::thread(&Actor::ThreadFunction, this);
}

void Actor::ThreadFunction()
{
    std::string greetings = std::string("I am the ") + mName + "!";
    mLogger->PushLogMessage(LogMessage(greetings, mLogEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Actor::StopThread()
{
    mActorThread.join();
}
