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
        auto ingredientInMeal = mIngredientsToPrepare.waitAndPop();
        if (!ingredientInMeal) break;
        mLogger->PushLogMessage(LogMessage("I am preparing an ingredient from " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mLogger->PushLogMessage(LogMessage("I prepared an ingredient from " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
        {
            mIngredientsReadyMutex.lock();
            auto& order = ingredientInMeal->first;
            order->mIngredientsReady.push_back(ingredientInMeal->second);
            if (ingredientInMeal->first->mIngredientsReady.size() == 3)
            {
                Meal meal("Meal", ingredientInMeal->first->mIngredientsReady);
                mMealToPrepare.push(std::pair<std::shared_ptr<Order>, Meal>(ingredientInMeal->first, meal));
                mLogger->PushLogMessage(LogMessage("I added a meal to be prepared for " + ingredientInMeal->first->mCustomer.lock()->mName, mLogEmitter));
            }
            mIngredientsReadyMutex.unlock();
        }
    }
}
