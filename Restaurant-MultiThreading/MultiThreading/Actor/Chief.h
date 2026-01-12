#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Chief : public Actor
{
protected:
    TSQueue<std::pair<Order*, Meal>>& mMealToPrepare;
    TSQueue<std::pair<Order*, Meal>>& mReadyMealQueue;
public:
    Chief(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<std::pair<Order*, Meal>>& mealToPrepare, TSQueue<std::pair<Order*, Meal>>& readyMealQueue);
    virtual ~Chief() override = default;
    virtual void ThreadFunction() override;
    
};
