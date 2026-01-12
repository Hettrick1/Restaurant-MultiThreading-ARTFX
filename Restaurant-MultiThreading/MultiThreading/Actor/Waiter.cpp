#include "Waiter.h"

#include "Customer.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

Waiter::Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter,std::shared_ptr<ILogger> logger,
    TSVector<std::shared_ptr<Order>>& orderQueue, TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<std::shared_ptr<Order>, Meal>>& readyMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue),
        mIngredientsToPrepare(ingredientsToPrepare), mReadyMealQueue(readyMealQueue)
{
}

void Waiter::ThreadFunction()
{
    Actor::ThreadFunction();
    while (mApplicationIsRunning)
    {
        // as the waiter has 2 tasks he needs to continuously check if he has something to do
        // if we don't do that we may have a deadlock situation
        std::shared_ptr<Order> order = nullptr;
        if (mOrderQueue.pop_back(order)) // try to get an order from the queue
        {
            // there is an order
            mLogger->PushLogMessage(LogMessage("I have receive an order, we need to prepare a " + order->mMeal.mName, mLogEmitter));
            for (auto i : order->mMeal.mIngredients.getCopy())
            {
                // push the ingredients to prepare in the queue
                mIngredientsToPrepare.push(std::pair<std::shared_ptr<Order>, Ingredient>(order, i));
            }
            continue;
        }
        std::shared_ptr<std::pair<std::shared_ptr<Order>, Meal>> ReadyMealPair = nullptr;
        if (mReadyMealQueue.try_pop(ReadyMealPair)) // try to get a meal from the ready meal queue
        {
            // a meal is ready
            mLogger->PushLogMessage(LogMessage("I am serving the " + ReadyMealPair->first->mMeal.mName + "...", mLogEmitter));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
            // send the meal for the customer that ordered it using the promise
            ReadyMealPair->first->mPromiseMeal.set_value(ReadyMealPair->second);
            
            mLogger->PushLogMessage(LogMessage("I have served the Meal!", mLogEmitter));
            continue;
        }
        // sleep a bit to save some CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
