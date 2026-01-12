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

    // no need for a loop here because the customer only handles himself

    // chose a random meal in the map
    Meal mealWanted = std::next(std::begin(Meal::AllMeals), intRand(0, 3))->second;
    // create the order
    Order order = Order(mealWanted, shared_from_this());
    
    mLogger->PushLogMessage(LogMessage("I am ordering " + mealWanted.mName + "!", mLogEmitter));
    // send the order in the queue
    mOrderQueue.push_back(std::make_shared<Order>(order));
    
    mLogger->PushLogMessage(LogMessage("I am waiting my meal...", mLogEmitter));
    
    // wait till the meal arrives
    // here we can and maybe should use a promise and a future because only one meal is ordered
    // with a future we could avoid sending the hole customer in the order, only the future ?
    
    std::shared_ptr<Meal> meal = mServedMealQueue.waitAndPop();
    if (!meal) return; // if the meal is nullptr, maybe that means the application is closing

    mLogger->PushLogMessage(LogMessage("I am eating " + meal->mName + "...", mLogEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mLogger->PushLogMessage(LogMessage("I am leaving!", mLogEmitter));
    // decrementing customer count
    --customer;
    if (customer == 0)
    {
        // when there are no more customer eating or waiting then we can close the application
        mApplicationIsRunning = false;
    }
}

void Customer::RecieveMeal(const Meal& meal)
{
    // recieve a meal
    mServedMealQueue.push(meal);
}

int Customer::intRand(const int & min, const int & max)
{
    // get a random int in a thread safe way using the thread id hash
    // the max is included
    static thread_local std::mt19937* generator = nullptr;
    std::hash<std::thread::id> hasher;
    if (!generator) generator = new std::mt19937(clock() + hasher(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}
