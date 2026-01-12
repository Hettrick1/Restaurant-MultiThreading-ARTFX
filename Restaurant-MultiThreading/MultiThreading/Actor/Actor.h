#pragma once

#include <thread>
#include "../ILogger.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"
#include "../TSQueue.h"
#include <utility>

// parent class of each actor
// handles a thread, so each actor is a thread
class Actor
{
protected:
    std::thread mActorThread;
    std::shared_ptr<LogEmitter>  mLogEmitter{nullptr};
    bool& mApplicationIsRunning;
    std::shared_ptr<ILogger> mLogger{nullptr};

    virtual void ThreadFunction();
    
public:
    std::string mName;
    Actor(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger);
    virtual ~Actor() = default;
    virtual void StartThread();
    virtual void StopThread();
};
