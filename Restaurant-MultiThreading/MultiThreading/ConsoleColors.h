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
    static const Color WHITE;
    static const Color BLACK;
    
    static const Color RED;
    static const Color GREEN;
    static const Color YELLOW;
    static const Color BLUE;
    static const Color MAGENTA;
    static const Color CYAN;
    static const Color GRAY;
    
    static const Color LIGHTRED;
    static const Color LIGHTGREEN;
    static const Color LIGHTYELLOW;
    static const Color LIGHTBLUE;
    static const Color LIGHTMAGENTA;
    static const Color LIGHTCYAN;
    static const Color LIGHTGRAY;
    
    static const Color DARKRED;    
    static const Color DARKGREEN; 
    static const Color DARKYELLOW;
    static const Color DARKBLUE; 
    static const Color DARKMAGENTA;
    static const Color DARKCYAN;
    static const Color DARKGRAY; 
    
    static const Color BOLDBLACK;
    static const Color BOLDRED;
    static const Color BOLDGREEN;
    static const Color BOLDYELLOW;
    static const Color BOLDBLUE;
    static const Color BOLDMAGENTA;
    static const Color BOLDCYAN;
    static const Color BOLDWHITE;
    static const Color BOLDGRAY;
};
