#include "Cooker.h"

#include "Customer.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

std::mutex Cooker::mIngredientsReadyMutex;

Cooker::Cooker(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger,
    TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mealToPrepare)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)),
        mIngredientsToPrepare(ingredientsToPrepare), mMealToPrepare(mealToPrepare)
{
}

void Cooker::ThreadFunction()
{
    Actor::ThreadFunction();
    while (mApplicationIsRunning)
    {
        // waits until there is an ingredient to prepare
        auto ingredientInMeal = mIngredientsToPrepare.waitAndPop();
        if (!ingredientInMeal) // if ingredientInMeal == nullptr, that means we should reconsider the while condition (maybe that means the application is closing)
        {
            break;
        }
        mLogger->PushLogMessage(LogMessage("I am preparing an ingredient from " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mLogger->PushLogMessage(LogMessage("I prepared an ingredient from " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
        {
            // lock the order we are working on
            std::lock_guard<std::mutex> guard(mIngredientsReadyMutex);
            auto& order = ingredientInMeal->first;
            
            // add the ingredient that is ready to the order's ingredient list
            order->mIngredientsReady.push_back(ingredientInMeal->second);
            if (ingredientInMeal->first->mIngredientsReady.size() == 3)
            {
                // all the ingredients are here so we can create a meal and push it in the meal to prepare queue
                // (maybe we should push the ingredient list in a queue and then make the chief create the meal ?)
                Meal meal = Meal::GetMeal(ingredientInMeal->first->mIngredientsReady);
                mMealToPrepare.push(std::pair<std::shared_ptr<Order>, Meal>(ingredientInMeal->first, meal));
                mLogger->PushLogMessage(LogMessage("I added a meal to be prepared for " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
            }
        }
    }
}
