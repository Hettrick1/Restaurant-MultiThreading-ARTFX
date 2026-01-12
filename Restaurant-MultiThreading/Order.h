#pragma once

class Customer;
#include "Meal.h"
#include <memory>
#include <future>

// handles orders taken by the customers
struct Order
{
    Meal mMeal;
    std::promise<Meal> mPromiseMeal;
    TSVector<Ingredient> mIngredientsReady;

    Order(Meal meal, std::promise<Meal>& promiseMeal)
        :  mMeal{std::move(meal)}, mPromiseMeal{std::move(promiseMeal)}
    {
        
    }
};
