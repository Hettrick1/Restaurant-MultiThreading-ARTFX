#include "Waiter.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

Waiter::Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter,std::shared_ptr<ILogger> logger,
    TSQueue<Order>& orderQueue, TSQueue<Ingredient>& ingredientsToPrepare, TSQueue<Meal>& readyMealQueue, TSQueue<Meal>& servedMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue),
        mIngredientsToPrepare(ingredientsToPrepare), mReadyMealQueue(readyMealQueue), mServedMealQueue(servedMealQueue)
{
}

void Waiter::ThreadFunction()
{
    Actor::ThreadFunction();
    while (mApplicationIsRunning)
    {
        std::shared_ptr<Order> order = nullptr;
        if (mOrderQueue.try_pop(order))
        {
            mLogger->PushLogMessage(LogMessage("I have receive an order!", mLogEmitter));
            for (Ingredient i : order->mIngredients.getCopy())
            {
                mIngredientsToPrepare.push(i);
            }
            continue;
        }
        std::shared_ptr<Meal> meal = nullptr;
        if (mReadyMealQueue.try_pop(meal))
        {
            mLogger->PushLogMessage(LogMessage("I am serving the Meal...", mLogEmitter));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            mServedMealQueue.push(*meal);
            mLogger->PushLogMessage(LogMessage("I have served the Meal!", mLogEmitter));
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
