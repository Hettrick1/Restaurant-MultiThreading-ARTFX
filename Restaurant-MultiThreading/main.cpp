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

static bool bIsRunning = true;

std::shared_ptr<LogEmitter> CustomerEmitter;
std::shared_ptr<LogEmitter> CustomerEmitter2;
std::shared_ptr<LogEmitter> CustomerEmitter3;
std::shared_ptr<LogEmitter> WaiterEmitter;
std::shared_ptr<LogEmitter> CookerEmitter;
std::shared_ptr<LogEmitter> CookerEmitter2;
std::shared_ptr<LogEmitter> ChiefEmitter;

std::shared_ptr<Customer> customer1;
std::shared_ptr<Customer> customer2;
std::shared_ptr<Customer> customer3;
std::unique_ptr<Waiter> waiter;
std::unique_ptr<Cooker> cooker;
std::unique_ptr<Cooker> cooker2;
std::unique_ptr<Chief> chief;


std::shared_ptr<TSLogger> Logger;

int main()
{
    CustomerEmitter = std::make_shared<LogEmitter>("Customer - 1", true, Color::BLUE);
    CustomerEmitter2 = std::make_shared<LogEmitter>("Customer - 2", true, Color::LIGHTBLUE);
    CustomerEmitter3 = std::make_shared<LogEmitter>("Customer - 3", true, Color::BOLDBLUE);
    WaiterEmitter = std::make_shared<LogEmitter>("Waiter", true, Color::LIGHTRED);
    CookerEmitter = std::make_shared<LogEmitter>("Cooker", true, Color::LIGHTGREEN);
    CookerEmitter2 = std::make_shared<LogEmitter>("Cooker - 2", true, Color::BOLDGREEN);
    ChiefEmitter = std::make_shared<LogEmitter>("Chief", true, Color::LIGHTYELLOW);

    Logger = std::make_shared<TSLogger>();

    Logger->StartLogging();
    
    Logger->PushLogMessage(LogMessage("Hello everybody, welcome in my new Restaurant multithreaded !", Color::WHITE));

    TSVector<Order*> orderQueue;
    TSQueue<std::pair<Order*, Ingredient>> ingredientsToPrepare;
    TSQueue<std::pair<Order*, Ingredient>> ingredientsReady;
    TSQueue<std::pair<Order*, Meal>> mealToPrepare;
    TSQueue<std::pair<Order*, Meal>> readyMealQueue;
    
    customer1 = std::make_shared<Customer>("Custommer1", bIsRunning, CustomerEmitter, Logger, orderQueue);
    customer2= std::make_shared<Customer>("Custommer2", bIsRunning, CustomerEmitter2, Logger, orderQueue);
    customer3= std::make_shared<Customer>("Custommer3", bIsRunning, CustomerEmitter3, Logger, orderQueue);
    waiter = std::make_unique<Waiter>("Waiter", bIsRunning, WaiterEmitter, Logger, orderQueue, ingredientsToPrepare, readyMealQueue);
    cooker = std::make_unique<Cooker>("Cooker", bIsRunning, CookerEmitter, Logger, ingredientsToPrepare, ingredientsReady, mealToPrepare);
    cooker2 = std::make_unique<Cooker>("Cooker2", bIsRunning, CookerEmitter2, Logger, ingredientsToPrepare, ingredientsReady, mealToPrepare);
    chief = std::make_unique<Chief>("Chief", bIsRunning, ChiefEmitter, Logger, mealToPrepare, readyMealQueue);

    customer1->StartThread();
    customer2->StartThread();
    customer3->StartThread();
    waiter->StartThread();
    cooker->StartThread();
    cooker2->StartThread();
    chief->StartThread();
    
    while (bIsRunning)
    {
        
    }
    
    ingredientsToPrepare.close();
    ingredientsReady.close();
    mealToPrepare.close();
    readyMealQueue.close();
    
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