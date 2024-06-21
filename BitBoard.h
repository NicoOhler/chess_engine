#pragma once
#include "definitions.h"
#include "utils.h"
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
const Position NO_EN_PASSANT = -100;

// castling
const Bitboard WHITE_KING_SIDE_CASTLING = 0x0000000000000060;
const Bitboard WHITE_QUEEN_SIDE_CASTLING = 0x000000000000000E;
const Bitboard BLACK_KING_SIDE_CASTLING = 0x6000000000000000;
const Bitboard BLACK_QUEEN_SIDE_CASTLING = 0x0E00000000000000;
const Bitboard WHITE_CASTLING = WHITE_KING_SIDE_CASTLING | WHITE_QUEEN_SIDE_CASTLING;
const Bitboard BLACK_CASTLING = BLACK_KING_SIDE_CASTLING | BLACK_QUEEN_SIDE_CASTLING;

// castling positions
const Position WHITE_KING_START_POSITION = 4;
const Position BLACK_KING_START_POSITION = 60;
const Position WHITE_KING_SIDE_ROOK_START_POSITION = 7;
const Position WHITE_QUEEN_SIDE_ROOK_START_POSITION = 0;
const Position BLACK_KING_SIDE_ROOK_START_POSITION = 63;
const Position BLACK_QUEEN_SIDE_ROOK_START_POSITION = 56;

namespace BitBoard
{
    struct Board
    {
        Board() : white_to_move(true),
                  white_pawns(WHITE_PAWNS_START), white_rooks(WHITE_ROOKS_START), white_knights(WHITE_KNIGHTS_START),
                  white_bishops(WHITE_BISHOPS_START), white_queens(WHITE_QUEENS_START), white_king(WHITE_KING_START),
                  black_pawns(BLACK_PAWNS_START), black_rooks(BLACK_ROOKS_START), black_knights(BLACK_KNIGHTS_START),
                  black_bishops(BLACK_BISHOPS_START), black_queens(BLACK_QUEENS_START), black_king(BLACK_KING_START),
                  castling_rights(WHITE_CASTLING | BLACK_CASTLING), occupied(WHITE_PIECES | BLACK_PIECES),
                  white_pieces(WHITE_PIECES), black_pieces(BLACK_PIECES), squares_under_attack(0) {}
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
        Bitboard white_pieces;
        Bitboard black_pieces;
        Bitboard squares_under_attack;
        Position en_passant;
        Bitboard castling_rights;
    };

    void movePiece(Bitboard &board, Position from, Position to);
    void set(Bitboard &board, Position position);
    void clear(Bitboard &board, Position position);
    bool isSet(Bitboard board, Position position);
    std::vector<bool> getBits(Bitboard board);
    void printBitboard(Bitboard board);
    int8 countSetBits(Bitboard board);
    Position clearRightmostSetBit(Bitboard &board); // sets the rightmost set bit to 0 and returns its index
}
