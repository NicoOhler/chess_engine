#include "BitBoard.h"

void BitBoard::set(Bitboard &board, uint8 position)
{
    board |= 1ULL << position;
}

void BitBoard::clear(Bitboard &board, uint8 position)
{
    board &= ~(1ULL << position);
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
