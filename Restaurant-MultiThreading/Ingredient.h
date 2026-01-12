#pragma once
#include <cstdint>
#include <string>

enum Ingredient : uint16_t
{
    STEAK = 1 << 0,
    FRIES = 1 << 1,
    SALT = 1 << 2,
    ZUCCHINI = 1 << 3,
    TOFU = 1 << 4,
    RICE = 1 << 5,
    PEPPER = 1 << 6,
    SMASHED_POTATOES = 1 << 7,
    NUGGETS = 1 << 8,
    CARROTS = 1 << 9,
};

inline Ingredient operator|(Ingredient a, Ingredient b)
{
    return static_cast<Ingredient>(static_cast<int>(a) | static_cast<int>(b));
}

namespace ingredients
{
    inline std::string to_string(Ingredient in)
    {
        switch (in)
        {
        case STEAK:
            return "STEAK";
        case FRIES:
            return "FRIES";
        case SALT:
            return "SALT";
        case ZUCCHINI:
            return "ZUCCHINI";
        case TOFU:
            return "TOFU";
        case RICE:
            return "RICE";
        case PEPPER:
            return "PEPPER";
        case SMASHED_POTATOES:
            return "SMASHED_POTATOES";
        case NUGGETS:
            return "NUGGETS";
        case CARROTS:
            return "CARROTS";
        default:
            return "";
        }
        return "";
    }
}

