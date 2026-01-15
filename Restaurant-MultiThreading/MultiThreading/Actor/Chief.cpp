#include "Chief.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

Chief::Chief(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger,
    TSQueue<std::pair<std::shared_ptr<Order>, TSVector<Ingredient>>>& mealToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& readyMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mMealToPrepare(mealToPrepare), mReadyMealQueue(readyMealQueue)
{
}

void Chief::ThreadFunction()
{
    Actor::ThreadFunction();
    while (mApplicationIsRunning)
    {
        // waits until there is a meal to prepare
        auto ingredientsInMeal = mMealToPrepare.waitAndPop();
        if (!ingredientsInMeal) // if meal == nullptr, that means we should reconsider the while condition (maybe that means the application is closing)
        {
            break;
        }

        Meal mealPrepared = Meal::GetMeal(ingredientsInMeal->first->mIngredientsReady);
        mLogger->PushLogMessage(LogMessage("I am preparing the meal...", mLogEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // the meal is now ready to be sent to the client
        mReadyMealQueue.push(std::pair<std::shared_ptr<Order>, Meal>(ingredientsInMeal->first, mealPrepared));
        mLogger->PushLogMessage(LogMessage("The " + ingredientsInMeal->first->mMeal.mName + " is ready!", mLogEmitter));
    }
}
