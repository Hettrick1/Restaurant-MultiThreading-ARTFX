#pragma once
#include "Ingredient.h"
#include "MultiThreading/TSVector.h"
#include <unordered_map>

struct Meal
{
    std::string mName;
    TSVector<Ingredient> mIngredients;

    Meal() = default;
    
    Meal(std::string name, const TSVector<Ingredient>& ingredients)
        : mName{std::move(name)}, mIngredients{ingredients}
    {
        
    }

    static Meal GetMeal(TSVector<Ingredient> ingredients);
    static std::unordered_map<uint16_t, Meal> AllMeals;
};
