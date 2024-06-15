#include "BitBoard.h"

Bitboard BitBoard::set(Bitboard board, uint8 position)
{
    return board | (1ULL << position);
}
/*
Bitboard BitBoard::set(Bitboard board, Bitboard position)
{
    return board | position;
}
*/

Bitboard BitBoard::clear(Bitboard board, uint8 position)
{
    return board & ~(1ULL << position);
}

/*
Bitboard BitBoard::clear(Bitboard board, Bitboard position)
{
    return board & ~position;
}
*/

bool BitBoard::isSet(Bitboard board, uint8 position)
{
    return board & (1ULL << position);
}