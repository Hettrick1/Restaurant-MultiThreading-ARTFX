#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class TSQueue
{
private:
    mutable std::mutex mut;
    std::queue<T> dataQueue;
    std::condition_variable dataCond;
public:
    TSQueue(){}
    
    TSQueue(const TSQueue& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        dataQueue = other.dataQueue;
    }
    
    TSQueue& operator=(const TSQueue&) = delete;
    
    void push(T newValue)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataQueue.push(newValue);
        dataCond.notify_one();
    }
    
    void waitAndPop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this] {return !dataQueue.empty();});
        value = dataQueue.front();
        dataQueue.pop();
    }
    
    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this] {return !dataQueue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(dataQueue.front()));
        dataQueue.pop();
        return res;
    }
    
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataQueue.empty())
            return false;
        value=dataQueue.front();
        dataQueue.pop();
        return true;
    }
    
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataQueue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared<T>(dataQueue.front()) );
        dataQueue.pop();
        return res;
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataQueue.empty();
    }
};