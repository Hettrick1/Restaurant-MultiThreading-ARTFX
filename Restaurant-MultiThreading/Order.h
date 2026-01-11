#pragma once
#include "Ingredient.h"
#include "MultiThreading/TSVector.h"

struct Order
{
    TSVector<Ingredient> mIngredients;

    Order(const TSVector<Ingredient>& ingredients)
        : mIngredients{ingredients}
    {
        
    }
};
