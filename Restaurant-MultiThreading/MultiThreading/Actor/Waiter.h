#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Waiter : public Actor
{
protected:
    TSQueue<Order>& mOrderQueue;
    TSQueue<Ingredient>& mIngredientsToPrepare;
    TSQueue<Meal>& mReadyMealQueue;
    TSQueue<Meal>& mServedMealQueue; 
public:
    Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<Order>& orderQueue, TSQueue<Ingredient>& ingredientsToPrepare, TSQueue<Meal>& readyMealQueue, TSQueue<Meal>& servedMealQueue);
    virtual ~Waiter() override = default;
    virtual void ThreadFunction() override;
};
