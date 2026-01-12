#pragma once
#include "Actor.h"

class Customer :  public std::enable_shared_from_this<Customer>, public Actor
{
protected:
    static std::atomic_int customer;

    TSVector<std::shared_ptr<Order>>& mOrderQueue;
    TSQueue<Meal> mServedMealQueue;

    int intRand(const int & min, const int & max);
public:
    Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSVector<std::shared_ptr<Order>>& orderQueue);
    virtual ~Customer() override = default;
    virtual void ThreadFunction() override;

    void RecieveMeal(const Meal& meal);
};
