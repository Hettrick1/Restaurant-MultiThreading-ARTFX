#pragma once
#include <memory>
#include <string>

#include "Actor.h"

// class that handles the waiter role (taking orders from the customers and bringing the meals to them when they are ready)
class Waiter : public Actor
{
protected:
    TSVector<std::shared_ptr<Order>>& mOrderQueue;
    TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& mIngredientsToPrepare;
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mReadyMealQueue;
public:
    Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSVector<std::shared_ptr<Order>>& orderQueue, TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& readyMealQueue);
    virtual ~Waiter() override = default;
    virtual void ThreadFunction() override;
};
