#pragma once
#include "Ingredient.h"
#include "MultiThreading/TSVector.h"

struct Meal
{
    std::string mName;
    TSVector<Ingredient> mIngredients;

    Meal() = default;
    Meal(std::string name, const TSVector<Ingredient>& ingredients)
        : mName{std::move(name)}, mIngredients{ingredients}
    {
        
    }

    static TSVector<Meal> AllMeals;
};
