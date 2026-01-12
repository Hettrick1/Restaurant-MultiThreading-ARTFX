#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

#include "MultiThreading/TSLogger.h"
#include "Ingredient.h"
#include "Meal.h"
#include "Order.h"
#include "MultiThreading/TSQueue.h"

#include "MultiThreading/Actor/Customer.h"
#include "MultiThreading/Actor/Waiter.h"
#include "MultiThreading/Actor/Cooker.h"
#include "MultiThreading/Actor/Chief.h"

#include <ctime>
#include <filesystem>
#include <utility>


// Here is the restaurant with 3 customers, 1 waiter, 2 cookers and 1 chief
// Every actor is its own thread
// normally it's working with any number of actor of each type

int main()
{
    bool bIsRunning = true;

    // actor emitters initialization
    std::shared_ptr<LogEmitter> CustomerEmitter = std::make_shared<LogEmitter>("Customer - 1", true, Color::BLUE);
    std::shared_ptr<LogEmitter> CustomerEmitter2 = std::make_shared<LogEmitter>("Customer - 2", true, Color::LIGHTBLUE);
    std::shared_ptr<LogEmitter> CustomerEmitter3 = std::make_shared<LogEmitter>("Customer - 3", true, Color::BOLDBLUE);
    std::shared_ptr<LogEmitter> WaiterEmitter = std::make_shared<LogEmitter>("Waiter", true, Color::LIGHTRED);
    std::shared_ptr<LogEmitter> CookerEmitter = std::make_shared<LogEmitter>("Cooker", true, Color::LIGHTGREEN);
    std::shared_ptr<LogEmitter> CookerEmitter2 = std::make_shared<LogEmitter>("Cooker - 2", true, Color::BOLDGREEN);
    std::shared_ptr<LogEmitter> ChiefEmitter = std::make_shared<LogEmitter>("Chief", true, Color::LIGHTYELLOW);

    // logger initialization
    std::shared_ptr<TSLogger> Logger = std::make_shared<TSLogger>();
    Logger->StartLogging();

    // print a message to test if it works
    Logger->PushLogMessage(LogMessage("Hello everybody, welcome in my new Restaurant multithreaded !"));

    TSVector<std::shared_ptr<Order>> orderQueue;
    TSQueue<std::pair<std::shared_ptr<Order>, Ingredient>> ingredientsToPrepare;
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>> mealToPrepare;
    TSQueue<std::pair<std::shared_ptr<Order>, Meal>> readyMealQueue;

    // actors initialization
    std::shared_ptr<Customer> customer1 = std::make_shared<Customer>("Customer1", bIsRunning, CustomerEmitter, Logger, orderQueue);
    std::shared_ptr<Customer> customer2= std::make_shared<Customer>("Customer2", bIsRunning, CustomerEmitter2, Logger, orderQueue);
    std::shared_ptr<Customer> customer3= std::make_shared<Customer>("Customer3", bIsRunning, CustomerEmitter3, Logger, orderQueue);
    std::unique_ptr<Waiter> waiter = std::make_unique<Waiter>("Waiter", bIsRunning, WaiterEmitter, Logger, orderQueue, ingredientsToPrepare, readyMealQueue);
    std::unique_ptr<Cooker> cooker = std::make_unique<Cooker>("Cooker", bIsRunning, CookerEmitter, Logger, ingredientsToPrepare, mealToPrepare);
    std::unique_ptr<Cooker> cooker2 = std::make_unique<Cooker>("Cooker2", bIsRunning, CookerEmitter2, Logger, ingredientsToPrepare, mealToPrepare);
    std::unique_ptr<Chief> chief = std::make_unique<Chief>("Chief", bIsRunning, ChiefEmitter, Logger, mealToPrepare, readyMealQueue);

    // start all threads
    customer1->StartThread();
    customer2->StartThread();
    customer3->StartThread();
    waiter->StartThread();
    cooker->StartThread();
    cooker2->StartThread();
    chief->StartThread();
    
    while (bIsRunning)
    {
        // we need a while loop but as we don't have any UI it is empty
    }

    // close the queue (wake up sleeping threads and then stop them) - we need that to avoid deadlock
    ingredientsToPrepare.close();
    mealToPrepare.close();
    readyMealQueue.close();

    // stop all threads
    Logger->StopLogging();
    customer1->StopThread();
    customer2->StopThread();
    customer3->StopThread();
    waiter->StopThread();
    cooker->StopThread();
    cooker2->StopThread();
    chief->StopThread();
    
    return 0;
}