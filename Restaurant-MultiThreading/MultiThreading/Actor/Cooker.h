#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Cooker : public Actor
{
protected:

    static std::mutex mIngredientsReadyMutex;
    
    TSQueue<std::pair<Order*, Ingredient>>& mIngredientsToPrepare;
    TSQueue<std::pair<Order*, Ingredient>>& mIngredientsReady;
    TSQueue<std::pair<Order*, Meal>>& mMealToPrepare;
public:
    Cooker(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<std::pair<Order*, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<Order*, Ingredient>>& ingredientsReady, TSQueue<std::pair<Order*, Meal>>& mealToPrepare);
    virtual ~Cooker() override = default;
    virtual void ThreadFunction() override;
};
