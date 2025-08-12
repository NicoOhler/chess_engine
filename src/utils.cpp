#include "utils.h"

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
