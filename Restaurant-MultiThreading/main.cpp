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

#include <ctime>

static TSQueue<Order> orderQueue;
static TSQueue<Ingredient> ingredientsToPrepare;
static TSQueue<Ingredient> ingredientsReady;
static TSQueue<Meal> mealToPrepare;
static TSQueue<Meal> readyMealQueue;
static TSQueue<Meal> servedMealQueue;     

static bool bIsRunning = true;

std::shared_ptr<LogEmitter> CustomerEmitter;
std::shared_ptr<LogEmitter> WaiterEmitter;
std::shared_ptr<LogEmitter> CookerEmitter;
std::shared_ptr<LogEmitter> ChiefEmitter;

std::unique_ptr<TSLogger> Logger;

void Customer()
{
    Logger->PushLogMessage( LogMessage("I am the customer!", CustomerEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    TSVector<Ingredient> ingredientsInMeal {Ingredient("Patatas"), Ingredient("Salt"), Ingredient("Peper")} ;
    Order order = Order(ingredientsInMeal);
    
    Logger->PushLogMessage(LogMessage("I am ordering!", CustomerEmitter));
    orderQueue.push(order);
    
    Logger->PushLogMessage(LogMessage("I am waiting my meal...", CustomerEmitter));
    std::shared_ptr<Meal> meal = servedMealQueue.waitAndPop();

    Logger->PushLogMessage(LogMessage("I am eating!", CustomerEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(2));
    Logger->PushLogMessage(LogMessage("I am leaving!", CustomerEmitter));
    bIsRunning = false;
}
void Waiter()
{
    Logger->PushLogMessage(LogMessage("I am the waiter!", WaiterEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::shared_ptr<Order> order = orderQueue.waitAndPop();
    Logger->PushLogMessage(LogMessage("I have receive an order!", WaiterEmitter));
    for (Ingredient i : order->mIngredients.getCopy())
    {
        ingredientsToPrepare.push(i);
    }
    std::shared_ptr<Meal> meal = readyMealQueue.waitAndPop();
    Logger->PushLogMessage(LogMessage("I am serving the Meal...", WaiterEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    servedMealQueue.push(*meal);
    Logger->PushLogMessage(LogMessage("I have served the Meal!", WaiterEmitter));
}
void Cooker()
{
    Logger->PushLogMessage(LogMessage("I am the cooker!", CookerEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (bIsRunning)
    {
        std::shared_ptr<Ingredient> ingredientInMeal = ingredientsToPrepare.waitAndPop();
        Logger->PushLogMessage(LogMessage("I am preparing an ingredient...", CookerEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ingredientsReady.push(*ingredientInMeal);
        if (ingredientsReady.size() == 3)
        {
            TSVector<Ingredient> ingredientsInMeal = {*ingredientsReady.waitAndPop(), *ingredientsReady.waitAndPop(), *ingredientsReady.waitAndPop()};
            Meal meal = Meal("Meal", ingredientsInMeal);
            mealToPrepare.push(meal);
            Logger->PushLogMessage(LogMessage("I added an meal to be prepared!", CookerEmitter));
        }
    }
}
void Chief()
{
    Logger->PushLogMessage(LogMessage("I am the chief!", ChiefEmitter));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (bIsRunning)
    {
        std::shared_ptr<Meal> meal = mealToPrepare.waitAndPop();
        Logger->PushLogMessage(LogMessage("I am preparing the meal...", ChiefEmitter));
        std::this_thread::sleep_for(std::chrono::seconds(3));
        readyMealQueue.push(*meal);
        Logger->PushLogMessage(LogMessage("The meal is ready!", ChiefEmitter));
    }
}

void LoggerThread()
{
    while (!Logger->PrintMessagesOrShouldStop());
}

int main()
{
    time_t start = time(0);

    CustomerEmitter = std::make_shared<LogEmitter>("Customer", true, Color::BLUE);
    WaiterEmitter = std::make_shared<LogEmitter>("Waiter", true, Color::RED);
    CookerEmitter = std::make_shared<LogEmitter>("Cooker", true, Color::GREEN);
    ChiefEmitter = std::make_shared<LogEmitter>("Chief", true, Color::YELLOW);

    Logger = std::make_unique<TSLogger>();

    std::thread logThread(LoggerThread);
    
    Logger->PushLogMessage(LogMessage("Hello everybody, welcome in my new Restaurant multithreaded !", Color::WHITE));
    
    std::thread CustomerThread = std::thread(Customer);
    std::thread WaiterThread = std::thread(Waiter);
    std::thread CookerThread = std::thread(Cooker);
    std::thread ChiefThread = std::thread(Chief);

    double seconds_since_start = 0;

    while (bIsRunning)
    {
        
    }

    Logger->StopLogging();
    logThread.join();
    
    CustomerThread.join();
    WaiterThread.join();
    CookerThread.join();
    ChiefThread.join();
    
    return 0;
}