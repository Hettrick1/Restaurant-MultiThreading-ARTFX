#pragma once
#include <memory>
#include <string>

#include "Actor.h"

// class that handles the chief role (creating meal based on ingredients prepared by the cooks)
class Chief : public Actor
{
protected:
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mMealToPrepare;
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mReadyMealQueue;
public:
    Chief(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mealToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& readyMealQueue);
    virtual ~Chief() override = default;
    virtual void ThreadFunction() override;
    
};
