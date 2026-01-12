#pragma once
#include <memory>
#include <string>

#include "Actor.h"

class Cooker : public Actor
{
protected:

    static std::mutex mIngredientsReadyMutex;
    
    TSQueue<Ingredient>& mIngredientsToPrepare;
    TSQueue<Ingredient>& mIngredientsReady;
    TSQueue<Meal>& mMealToPrepare;
public:
    Cooker(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<Ingredient>& ingredientsToPrepare, TSQueue<Ingredient>& ingredientsReady, TSQueue<Meal>& mealToPrepare);
    virtual ~Cooker() override = default;
    virtual void ThreadFunction() override;
};
