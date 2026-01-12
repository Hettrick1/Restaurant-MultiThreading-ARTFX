#include "Chief.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

Chief::Chief(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger,
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& mealToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& readyMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mMealToPrepare(mealToPrepare), mReadyMealQueue(readyMealQueue)
{
}

void Chief::ThreadFunction()
{
    Actor::ThreadFunction();
    
    while (mApplicationIsRunning)
    {
        auto meal = mMealToPrepare.waitAndPop();
        if (!meal) break;
        mLogger->PushLogMessage(LogMessage("I am preparing the meal...", mLogEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        mReadyMealQueue.push(*meal);
        mLogger->PushLogMessage(LogMessage("The meal is ready!", mLogEmitter));
    }
}
