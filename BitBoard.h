#pragma once
#include "definitions.h"
#include <iostream>
#include <vector>

const Bitboard WHITE_PAWNS_START = 0x000000000000FF00;
const Bitboard WHITE_KNIGHTS_START = 0x0000000000000042;
const Bitboard WHITE_BISHOPS_START = 0x0000000000000024;
const Bitboard WHITE_ROOKS_START = 0x0000000000000081;
const Bitboard WHITE_QUEENS_START = 0x0000000000000008;
const Bitboard WHITE_KING_START = 0x0000000000000010;

const Bitboard BLACK_PAWNS_START = 0x00FF000000000000;
const Bitboard BLACK_KNIGHTS_START = 0x4200000000000000;
const Bitboard BLACK_BISHOPS_START = 0x2400000000000000;
const Bitboard BLACK_ROOKS_START = 0x8100000000000000;
const Bitboard BLACK_QUEENS_START = 0x0800000000000000;
const Bitboard BLACK_KING_START = 0x1000000000000000;

const Bitboard WHITE_PIECES = 0x000000000000FFFF;
const Bitboard BLACK_PIECES = 0xFFFF000000000000;

namespace BitBoard
{
    struct Board
    {
        Board() : white_to_move(true), white_pawns(WHITE_PAWNS_START), white_rooks(WHITE_ROOKS_START), white_knights(WHITE_KNIGHTS_START), white_bishops(WHITE_BISHOPS_START), white_queens(WHITE_QUEENS_START), white_king(WHITE_KING_START), black_pawns(BLACK_PAWNS_START), black_rooks(BLACK_ROOKS_START), black_knights(BLACK_KNIGHTS_START), black_bishops(BLACK_BISHOPS_START), black_queens(BLACK_QUEENS_START), black_king(BLACK_KING_START), occupied(WHITE_PIECES | BLACK_PIECES), occupied_by_white(WHITE_PIECES), occupied_by_black(BLACK_PIECES), squares_under_attack(0) {}
        bool white_to_move;
        Bitboard white_pawns;
        Bitboard white_rooks;
        Bitboard white_knights;
        Bitboard white_bishops;
        Bitboard white_queens;
        Bitboard white_king;

        Bitboard black_pawns;
        Bitboard black_rooks;
        Bitboard black_knights;
        Bitboard black_bishops;
        Bitboard black_queens;
        Bitboard black_king;

        Bitboard occupied;
        Bitboard occupied_by_white;
        Bitboard occupied_by_black;
        Bitboard squares_under_attack;
    };

    void set(Bitboard &board, uint8 position);
    void clear(Bitboard &board, uint8 position);
    bool isSet(Bitboard board, uint8 position);
    std::vector<bool> getBits(Bitboard board);
    void printBitboard(Bitboard board);
    int8 countSetBits(Bitboard board);
    Position clearRightmostSetBit(Bitboard &board); // sets the rightmost set bit to 0 and returns its index
}
