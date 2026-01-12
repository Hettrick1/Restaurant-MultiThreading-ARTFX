#include "Meal.h"

TSVector<Meal> Meal::AllMeals = {
    Meal("Steak Fries", {Ingredient("Steak"), Ingredient("Fries"), Ingredient("Salt")}),
    Meal("Veggie Meal", {Ingredient("Zuchini"), Ingredient("Tofu"), Ingredient("Rice")}),
    Meal("Peper Steak", {Ingredient("Steak"), Ingredient("Peper"), Ingredient("Smashed Potatoes")}),
    Meal("Children Meal", {Ingredient("Nugget"), Ingredient("Carrot"), Ingredient("Smashed Potatoes")})
};