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
    bool mClosed = false;
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
    
    std::shared_ptr<T> waitAndPop()
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this] {return mClosed || !dataQueue.empty();});

        if (mClosed && dataQueue.empty())
        {
            return nullptr;
        }
        
        std::shared_ptr<T> res(std::make_shared<T>(dataQueue.front()));
        dataQueue.pop();
        return res;
    }
    
    bool try_pop(std::shared_ptr<T>& valuePtr)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataQueue.empty())
        {
            return false;
        }
        valuePtr = std::make_shared<T>(std::move(dataQueue.front()));
        dataQueue.pop();
        return true;
    }
    
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataQueue.empty())
        {
            return nullptr;
        }
        std::shared_ptr<T> res( std::make_shared<T>(dataQueue.front()) );
        dataQueue.pop();
        return res;
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataQueue.size();
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataQueue.empty();
    }

    void close()
    {
        {
            std::lock_guard<std::mutex> lk(mut);
            mClosed = true;
        }
        dataCond.notify_all();
    }
};