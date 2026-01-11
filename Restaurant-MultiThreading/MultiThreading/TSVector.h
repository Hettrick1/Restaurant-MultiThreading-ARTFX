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
public:
    TSVector(){}
    
    TSVector(const TSVector& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        dataVector = other.dataVector;
    }
    
    TSVector(std::initializer_list<T> init)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataVector = init;
    }

    std::vector<T> getCopy()
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataVector;
    }
    
    TSVector& operator=(const TSVector&) = delete;
    
    void push_back(T newValue)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataVector.push_back(std::move(newValue));
    }

    void push_back(std::initializer_list<T> ilist)
    {
        std::lock_guard<std::mutex> lk(mut);
        dataVector.insert(dataVector.end(), ilist);
    }
    
    std::shared_ptr<T> pop_back()
    {
        std::unique_lock<std::mutex> lk(mut);
        if (dataVector.empty())
        {
            return nullptr;
        }
        std::shared_ptr<T> res(std::make_shared<T>(std::move(dataVector.back())));
        dataVector.pop_back();
        return res;
    }
    
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return dataVector.empty();
    }
};
