#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Waiter : public Actor
{
protected:
    TSVector<Order*>& mOrderQueue;
    TSQueue<std::pair<Order*, Ingredient>>& mIngredientsToPrepare;
    TSQueue<std::pair<Order*, Meal>>& mReadyMealQueue;
public:
    Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSVector<Order*>& orderQueue, TSQueue<std::pair<Order*, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<Order*, Meal>>& readyMealQueue);
    virtual ~Waiter() override = default;
    virtual void ThreadFunction() override;
};
