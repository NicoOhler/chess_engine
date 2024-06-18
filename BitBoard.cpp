#include "BitBoard.h"

void BitBoard::movePiece(Bitboard &board, Position from, Position to)
{
    BitBoard::clear(board, from);
    BitBoard::set(board, to);
}

void BitBoard::set(Bitboard &board, Position position)
{
    board |= 1ULL << position;
}

void BitBoard::clear(Bitboard &board, Position position)
{
    board &= ~(1ULL << position);
}

bool BitBoard::isSet(Bitboard board, Position position)
{
    return board & (1ULL << position);
}

std::vector<bool> BitBoard::getBits(Bitboard board)
{
    std::vector<bool> bits(NUM_SQUARES);
    for (int i = 0; i < NUM_SQUARES; i++)
        bits[i] = board & (1ULL << i);
    return bits;
}
void BitBoard::printBitboard(Bitboard board)
{
    std::vector<bool> bits = getBits(board);
    std::cout << std::endl;
    for (Position row = NUM_ROWS - 1; row >= 0; row--)
    {
        std::cout << row + 1 << " ";
        for (Position col = 0; col < NUM_COLS; col++)
            std::cout << bits[row * NUM_COLS + col] ? "1" : "0";
        std::cout << std::endl;
    }
    std::cout << "  ";
    for (Position col = 0; col < NUM_COLS; col++)
        std::cout << (char)('a' + col);
    std::cout << std::endl
              << std::endl;
}
int8 BitBoard::countSetBits(Bitboard board)
{
    return __builtin_popcountll(board);
}
Position BitBoard::clearRightmostSetBit(Bitboard &board)
{
    Position index_of_LSB = __builtin_ctzll(board);
    board &= board - 1;
    return index_of_LSB;
}
