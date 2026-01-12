#include "Cooker.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

std::mutex Cooker::mIngredientsReadyMutex;

Cooker::Cooker(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger,
    TSQueue<Ingredient>& ingredientsToPrepare, TSQueue<Ingredient>& ingredientsReady, TSQueue<Meal>& mealToPrepare)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)),
        mIngredientsToPrepare(ingredientsToPrepare), mIngredientsReady(ingredientsReady), mMealToPrepare(mealToPrepare)
{
}

void Cooker::ThreadFunction()
{
    Actor::ThreadFunction();

    while (mApplicationIsRunning)
    {
        std::shared_ptr<Ingredient> ingredientInMeal = mIngredientsToPrepare.waitAndPop();
        if (!ingredientInMeal) break;
        mLogger->PushLogMessage(LogMessage("I am preparing an ingredient...", mLogEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std::mutex> lk(mIngredientsReadyMutex);
            mIngredientsReady.push(*ingredientInMeal);
            if (mIngredientsReady.size() >= 3)
            {
                TSVector<Ingredient> ingredientsInMeal;
                std::shared_ptr<Ingredient> ingredientReady = nullptr;
                for (int i = 0; i < 3; ++i)
                {
                    ingredientReady = mIngredientsReady.try_pop();
                    if (!ingredientReady)
                    {
                        break;
                    }
                    ingredientsInMeal.push_back(*ingredientReady);
                }
                Meal meal("Meal", ingredientsInMeal);
                mMealToPrepare.push(meal);
                mLogger->PushLogMessage(LogMessage("I added a meal to be prepared!", mLogEmitter));
            }
        }
    }
}
