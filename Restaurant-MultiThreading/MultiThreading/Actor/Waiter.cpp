#include "Waiter.h"

#include "Customer.h"
#include "../../Ingredient.h"
#include "../../Order.h"
#include "../../Meal.h"
#include "../TSVector.h"

Waiter::Waiter(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter,std::shared_ptr<ILogger> logger,
    TSVector<Order*>& orderQueue, TSQueue<std::pair<Order*, Ingredient>>& ingredientsToPrepare, TSQueue<std::pair<Order*, Meal>>& readyMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue),
        mIngredientsToPrepare(ingredientsToPrepare), mReadyMealQueue(readyMealQueue)
{
}

void Waiter::ThreadFunction()
{
    Actor::ThreadFunction();
    while (mApplicationIsRunning)
    {
        Order* order = nullptr;
        if (mOrderQueue.pop_back(order))
        {
            mLogger->PushLogMessage(LogMessage("I have receive an order from " + order->mCustomer.lock()->mName + "!" , mLogEmitter));
            for (auto i : order->mMeal.mIngredients.getCopy())
            {
                mIngredientsToPrepare.push(std::pair<Order*, Ingredient>(order, i));
            }
            continue;
        }
        std::shared_ptr<std::pair<Order*, Meal>> orderMealPair = nullptr;
        if (mReadyMealQueue.try_pop(orderMealPair))
        {
            mLogger->PushLogMessage(LogMessage("I am serving the Meal to" + orderMealPair->first->mCustomer.lock()->mName + "...", mLogEmitter));
            std::this_thread::sleep_for(std::chrono::seconds(1));
            orderMealPair->first->mCustomer.lock()->RecieveMeal(orderMealPair->first->mMeal);
            mLogger->PushLogMessage(LogMessage("I have served the Meal!", mLogEmitter));
            continue;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
