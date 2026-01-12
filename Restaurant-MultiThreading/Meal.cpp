#include "Meal.h"

std::unordered_map<uint16_t, Meal> Meal::AllMeals = {
    // creates meal templates
    {Ingredient::STEAK | Ingredient::FRIES | Ingredient::SALT, Meal("Steak Fries", {Ingredient::STEAK, Ingredient::FRIES, Ingredient::SALT})},
    {Ingredient::ZUCHINI | Ingredient::TOFU | Ingredient::RICE, Meal("Veggie Meal", {Ingredient::ZUCHINI, Ingredient::TOFU, Ingredient::RICE})},
    {Ingredient::STEAK | Ingredient::PEPER | Ingredient::SMASHED_POTATOES, Meal("Peper Steak", {Ingredient::STEAK, Ingredient::PEPER, Ingredient::SMASHED_POTATOES})},
    {Ingredient::NUGGETS | Ingredient::CARROTS | Ingredient::SMASHED_POTATOES, Meal("Children Meal", {Ingredient::NUGGETS, Ingredient::CARROTS, Ingredient::SMASHED_POTATOES})}
};

Meal Meal::GetMeal(TSVector<Ingredient> ingredients)
{
    // returns a meal base on its ingredients (if no meal corresponds it returns a default meal)
    
    uint16_t id = 0;
    for (auto ingredient : ingredients.getCopy())
    {
        id |= ingredient;
    }
    
    if (AllMeals.contains(id))
    {
        return AllMeals[id];
    }
    else
    {
        return Meal("Default Meal", {Ingredient::STEAK, Ingredient::FRIES, Ingredient::SALT});
    }
}
