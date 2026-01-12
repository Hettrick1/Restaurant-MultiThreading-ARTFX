#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Chief : public Actor
{
protected:
    TSQueue<Meal>& mMealToPrepare;
    TSQueue<Meal>& mReadyMealQueue;
public:
    Chief(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<Meal>& mealToPrepare, TSQueue<Meal>& readyMealQueue);
    virtual ~Chief() override = default;
    virtual void ThreadFunction() override;
    
};
