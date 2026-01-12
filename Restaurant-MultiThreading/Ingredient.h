#pragma once
#include <cstdint>

enum Ingredient : uint16_t
{
    STEAK = 1 << 0,
    FRIES = 1 << 1,
    SALT = 1 << 2,
    ZUCHINI = 1 << 3,
    TOFU = 1 << 4,
    RICE = 1 << 5,
    PEPER = 1 << 6,
    SMASHED_POTATOES = 1 << 7,
    NUGGETS = 1 << 8,
    CARROTS = 1 << 9,
};

inline Ingredient operator|(Ingredient a, Ingredient b)
{
    return static_cast<Ingredient>(static_cast<int>(a) | static_cast<int>(b));
}
