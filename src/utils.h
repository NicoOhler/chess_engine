#pragma once

#include "definitions.h"
#include <string>
#include <iostream>

#define assert(condition, message)         \
    if (!(condition))                      \
    {                                      \
        std::cerr << message << std::endl; \
        exit(1);                           \
    }

std::string getSquareName(Position square);
Position getSquareIndex(std::string square);
