#include "Customer.h"

#include <random>
#include <time.h>
#include <thread>

std::atomic_int Customer::customer = 0;

Customer::Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger, TSVector<std::shared_ptr<Order>>& orderQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue)
{
    ++customer;
}

void Customer::ThreadFunction()
{
    Actor::ThreadFunction();

    Meal mealWanted = *Meal::AllMeals[intRand(0, 3)];
    TSVector<Ingredient> ingredientsInMeal = mealWanted.mIngredients ;
    Order order = Order(mealWanted, shared_from_this());
    
    mLogger->PushLogMessage(LogMessage("I am ordering " + mealWanted.mName + "!", mLogEmitter));
    mOrderQueue.push_back(std::make_shared<Order>(order));
    
    mLogger->PushLogMessage(LogMessage("I am waiting my meal...", mLogEmitter));
    std::shared_ptr<Meal> meal = mServedMealQueue.waitAndPop();
    if (!meal) return;

    mLogger->PushLogMessage(LogMessage("I am eating " + meal->mName + "...", mLogEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mLogger->PushLogMessage(LogMessage("I am leaving!", mLogEmitter));
    --customer;
    if (customer == 0)
    {
        mApplicationIsRunning = false;
    }
}

void Customer::RecieveMeal(const Meal& meal)
{
    mServedMealQueue.push(meal);
}

int Customer::intRand(const int & min, const int & max)
{
    static thread_local std::mt19937* generator = nullptr;
    std::hash<std::thread::id> hasher;
    if (!generator) generator = new std::mt19937(clock() + hasher(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}
