#include "BitBoard.h"

Bitboard BitBoard::set(Bitboard board, uint8 position)
{
    return board | (1ULL << position);
}

Bitboard BitBoard::clear(Bitboard board, uint8 position)
{
    return board & ~(1ULL << position);
}

bool BitBoard::isSet(Bitboard board, uint8 position)
{
    return board & (1ULL << position);
}

Position BitBoard::clearRightmostSetBit(Bitboard &board)
{
    Position index_of_LSB = __builtin_ctzll(board);
    board &= board - 1;
    return index_of_LSB;
}
