#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "MultiThreading/TSLogger.h"
#include "Ingredient.h"
#include "MultiThreading/TSQueue.h"

#include <ctime>

static TSQueue<Ingredient> ingredientsQueue;

static bool bIsRunning = true;

std::shared_ptr<LogEmitter> CustomerEmitter;
std::shared_ptr<LogEmitter> WaiterEmitter;
std::shared_ptr<LogEmitter> CookerEmitter;
std::shared_ptr<LogEmitter> ChiefEmitter;

std::unique_ptr<TSLogger> Logger;

void Customer()
{
    while (bIsRunning)
    {
        LogMessage message = LogMessage("I am the customer !", CustomerEmitter);
        Logger->PushLogMessage(message);
        Ingredient newIngredient;
        newIngredient.name = "Patatas";
        ingredientsQueue.push(newIngredient);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void Waiter()
{
    while (bIsRunning)
    {
        LogMessage message = LogMessage("I am the waiter !", WaiterEmitter);
        Logger->PushLogMessage(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void Cooker()
{
    while (bIsRunning)
    {
        LogMessage message = LogMessage("I am the cooker !", CookerEmitter);
        Logger->PushLogMessage(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
void Chief()
{
    while (bIsRunning)
    {
        LogMessage message = LogMessage("I am the chief !", ChiefEmitter);
        Logger->PushLogMessage(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main()
{
    std::cout << "Hello everybody, welcome in my new Restaurant multithreaded !" << std::endl;
    
    time_t start = time(0);

    CustomerEmitter = std::make_shared<LogEmitter>("Customer", false, Color::BLUE);
    WaiterEmitter = std::make_shared<LogEmitter>("Waiter", false, Color::RED);
    CookerEmitter = std::make_shared<LogEmitter>("Cooker", false, Color::GREEN);
    ChiefEmitter = std::make_shared<LogEmitter>("Chief", false, Color::YELLOW);

    Logger = std::make_unique<TSLogger>();
    
    std::thread CustomerThread = std::thread(Customer);
    std::thread WaiterThread = std::thread(Waiter);
    std::thread CookerThread = std::thread(Cooker);
    std::thread ChiefThread = std::thread(Chief);

    double seconds_since_start = 0;
    
    while (seconds_since_start < 10)
    {
        seconds_since_start = difftime( time(0), start);
        Logger->PrintMessages();
    }
    bIsRunning = false;
    
    CustomerThread.join();
    WaiterThread.join();
    CookerThread.join();
    ChiefThread.join();
    
    return 0;
}