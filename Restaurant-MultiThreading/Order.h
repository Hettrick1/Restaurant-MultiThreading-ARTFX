#pragma once

class Customer;
#include "Meal.h"
#include <memory>

// handles orders taken by the customers
struct Order
{
    std::weak_ptr<Customer> mCustomer; // maybe we could use a future here
    TSVector<Ingredient> mIngredientsReady;
    Meal mMeal;

    Order(Meal meal, const std::shared_ptr<Customer>& customer)
        :  mCustomer {customer}, mMeal{std::move(meal)}
    {
        
    }
};
