#include "utils.h"

std::string getSquareName(Position square)
{
    if (square < 0 || square > 63)
        return "Invalid square";

    return char('a' + square % 8) + std::to_string(square / 8 + 1);
}

Position getSquareIndex(std::string square)
{
    if (square.length() != 2)
        return -1;

    char col = square[0];
    char row = square[1];

    if (col < 'a' || col > 'h' || row < '1' || row > '8')
        return -1;

    return 8 * (row - '1') + (col - 'a');
}
