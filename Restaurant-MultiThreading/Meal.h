#pragma once
#include "Ingredient.h"
#include "MultiThreading/TSVector.h"

struct Meal
{
    std::string name;
    TSVector<Ingredient> ingredients;
};
