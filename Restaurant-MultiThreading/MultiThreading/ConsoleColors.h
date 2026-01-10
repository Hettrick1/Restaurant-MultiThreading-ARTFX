#pragma once
#include <string>

struct Color
{
    std::string mColorCode;

    Color(const std::string& ansiColor)
        : mColorCode{ ansiColor }
    {
        
    }

    static const Color RESET;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color YELLOW;
    static const Color BLUE;
    static const Color MAGENTA;
    static const Color CYAN;
    static const Color WHITE;
    static const Color BOLDBLACK;
    static const Color BOLDRED;
    static const Color BOLDGREEN;
    static const Color BOLDYELLOW;
    static const Color BOLDBLUE;
    static const Color BOLDMAGENTA;
    static const Color BOLDCYAN;
    static const Color BOLDWHITE;
};
