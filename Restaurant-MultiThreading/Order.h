#pragma once

class Customer;
#include "Meal.h"
#include <memory>

struct Order
{
    std::weak_ptr<Customer> mCustomer;
    TSVector<Ingredient> mIngredientsReady;
    Meal mMeal;

    Order(Meal meal, const std::shared_ptr<Customer>& customer)
        :  mCustomer {customer}, mMeal{std::move(meal)}
    {
        
    }
};
