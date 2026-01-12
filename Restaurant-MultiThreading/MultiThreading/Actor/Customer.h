#pragma once
#include "Actor.h"

class Customer : public Actor
{
protected:
    static std::atomic_int customer;

    TSQueue<Order>& mOrderQueue;
    TSQueue<Meal>& mServedMealQueue;
public:
    Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSQueue<Order>& orderQueue, TSQueue<Meal>& servedMealQueue);
    virtual ~Customer() override = default;
    virtual void ThreadFunction() override;
};
