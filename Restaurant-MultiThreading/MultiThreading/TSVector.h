#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

template<typename T>
class TSVector
{
private:
    mutable std::mutex mut;
    std::vector<T> dataVector;
    std::condition_variable dataCond;
public:
    TSVector(){}
    
    TSVector(const TSVector& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        dataVector = other.dataVector;
    }
    
    TSVector& operator=(const TSVector&) = delete;
    
    void push_back(T newValue)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataVector.push_back(newValue);
        dataCond.notify_one();
    }
    
    void waitAndPop_back(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this] {return !dataVector.empty();});
        value = dataVector.back();
        dataVector.pop_back();
    }
    
    std::shared_ptr<T> waitAndPop_back()
    {
        std::unique_lock<std::mutex> lk(mut);
        dataCond.wait(lk, [this] {return !dataVector.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(dataVector.back()));
        dataVector.pop_back();
        return res;
    }
    
    bool try_pop_back(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataVector.empty())
            return false;
        value = dataVector.back();
        dataVector.pop_back();
        return true;
    }
    
    std::shared_ptr<T> try_pop_back()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(dataVector.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res( std::make_shared<T>(dataVector.back()) );
        dataVector.pop_back();
        return res;
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataVector.empty();
    }
};
