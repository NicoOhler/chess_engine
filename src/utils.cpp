#include "utils.h"
#include <chrono>

std::string getSquareName(Position square)
{
    assert(square >= 0 && square < 64, "Invalid square");
    return char('a' + square % 8) + std::to_string(square / 8 + 1);
}

Position getSquareIndex(std::string square)
{
    assert(square.length() == 2, "Invalid square");
    char col = square[0];
    char row = square[1];
    assert(col >= 'a' && col <= 'h' && row >= '1' && row <= '8', "Invalid square");
    return 8 * (row - '1') + (col - 'a');
}

uint64 getCurrentTimeSeconds()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

// return number of hours, minutes and seconds
std::string convertSecondsToString(uint64 seconds)
{
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;
    return std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
}
