#pragma once
#include "Actor.h"

class Customer :  public std::enable_shared_from_this<Customer>, public Actor
{
protected:
    static std::atomic_int customer;

    TSVector<Order*>& mOrderQueue;
    TSQueue<Meal> mServedMealQueue;
public:
    Customer(std::string name, bool& applicationIsRunning, std::shared_ptr<LogEmitter>  logEmitter, std::shared_ptr<ILogger> logger, TSVector<Order*>& orderQueue);
    virtual ~Customer() override = default;
    virtual void ThreadFunction() override;

    void RecieveMeal(const Meal& meal);
};
