#include "Customer.h"

#include <random>
#include <time.h>
#include <thread>

std::atomic_int Customer::customer = 0;

Customer::Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter> logEmitter, std::shared_ptr<ILogger> logger, TSVector<std::shared_ptr<Order>>& orderQueue)
    : Actor(std::move(name), applicationIsRunning, std::move(logEmitter), std::move(logger)), mOrderQueue(orderQueue)
{
    ++customer;
    mFutureMeal = mPromisedMeal.get_future();
}

void Customer::ThreadFunction()
{
    Actor::ThreadFunction();

    // no need for a loop here because the customer only handles himself

    // chose a random meal in the map
    Meal mealWanted = std::next(std::begin(Meal::AllMeals), intRand(0, 3))->second;
    // create the order
    std::shared_ptr<Order> order = std::make_shared<Order>(mealWanted,std::ref(mPromisedMeal));
    
    mLogger->PushLogMessage(LogMessage("I am ordering " + mealWanted.mName + "!", mLogEmitter));
    // send the order in the queue
    mOrderQueue.push_back(order);
    
    mLogger->PushLogMessage(LogMessage("I am waiting my meal...", mLogEmitter));
    
    // wait till the meal arrives
    // here I use a promise and a future to avoid sending the whole customer in the order
    Meal meal = mFutureMeal.get();

    mLogger->PushLogMessage(LogMessage("I am eating " + meal.mName + "...", mLogEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mLogger->PushLogMessage(LogMessage("I am leaving!", mLogEmitter));
    
    // decrementing customer count
    --customer;
    if (customer == 0)
    {
        // when there are no more customer eating or waiting then we can close the application
        mLogger->PushLogMessage(LogMessage("All clients have exit the restaurant"));
        mApplicationIsRunning = false;
    }
}

int Customer::intRand(const int & min, const int & max)
{
    // get a random int in a thread safe way using the thread id hash
    // the max is included
    static thread_local std::mt19937* generator = nullptr;
    std::hash<std::thread::id> hasher;
    if (!generator) generator = new std::mt19937(static_cast<unsigned int>(clock() + hasher(std::this_thread::get_id())));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}
