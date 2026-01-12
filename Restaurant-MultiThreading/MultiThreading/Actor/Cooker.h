#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Cooker : public Actor
{
protected:

    static std::mutex mIngredientsReadyMutex;
    
    TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& mIngredientsToPrepare;
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mMealToPrepare;
public:
    Cooker(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mealToPrepare);
    virtual ~Cooker() override = default;
    virtual void ThreadFunction() override;
};
