#include "Customer.h"

std::atomic_int Customer::customer = 0;

Customer::Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger, TSQueue<Order>& orderQueue, TSQueue<Meal>& servedMealQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue), mServedMealQueue(servedMealQueue)
{
    ++customer;
}

void Customer::ThreadFunction()
{
    Actor::ThreadFunction();
    
    TSVector<Ingredient> ingredientsInMeal {Ingredient("Patatas"), Ingredient("Salt"), Ingredient("Peper")} ;
    Order order = Order(ingredientsInMeal);
    
    mLogger->PushLogMessage(LogMessage("I am ordering!", mLogEmitter));
    mOrderQueue.push(order);
    
    mLogger->PushLogMessage(LogMessage("I am waiting my meal...", mLogEmitter));
    std::shared_ptr<Meal> meal = mServedMealQueue.waitAndPop();
    if (!meal) return;

    mLogger->PushLogMessage(LogMessage("I am eating...", mLogEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mLogger->PushLogMessage(LogMessage("I am leaving!", mLogEmitter));
    --customer;
    if (customer == 0)
    {
        mApplicationIsRunning = false;
    }
}
