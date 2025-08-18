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

uint64 getCurrentTimeMilliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

// return number of hours, minutes, seconds and milliseconds
std::string convertMillisecondsToString(uint64 milliseconds)
{
    int hours = milliseconds / 3600000;
    milliseconds %= 3600000;
    int minutes = milliseconds / 60000;
    milliseconds %= 60000;
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;
    return std::to_string(hours) + "h " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s " + std::to_string(milliseconds) + "ms";
}
