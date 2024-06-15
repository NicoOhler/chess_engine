#pragma once
#include "definitions.h"

#define BOARD_SIZE 8

#define WHITE_PAWNS_START 0x000000000000FF00
#define WHITE_KNIGHTS_START 0x0000000000000042
#define WHITE_BISHOPS_START 0x0000000000000024
#define WHITE_ROOKS_START 0x0000000000000081
#define WHITE_QUEENS_START 0x0000000000000008
#define WHITE_KING_START 0x0000000000000010

#define BLACK_PAWNS_START 0x00FF000000000000
#define BLACK_KNIGHTS_START 0x4200000000000000
#define BLACK_BISHOPS_START 0x2400000000000000
#define BLACK_ROOKS_START 0x8100000000000000
#define BLACK_QUEENS_START 0x0800000000000000
#define BLACK_KING_START 0x1000000000000000

namespace BitBoard
{
    struct Board
    {
        Board() : white_to_move(true), white_pawns(WHITE_PAWNS_START), white_rooks(WHITE_ROOKS_START), white_knights(WHITE_KNIGHTS_START), white_bishops(WHITE_BISHOPS_START), white_queens(WHITE_QUEENS_START), white_king(WHITE_KING_START), black_pawns(BLACK_PAWNS_START), black_rooks(BLACK_ROOKS_START), black_knights(BLACK_KNIGHTS_START), black_bishops(BLACK_BISHOPS_START), black_queens(BLACK_QUEENS_START), black_king(BLACK_KING_START), occupied(0), occupied_by_white(0), occupied_by_black(0), squares_under_attack(0) {}
        bool white_to_move;
        /*
        int castling;
        int epSquare;
        int halfmoves;
        int fullmoves;
        Position whiteKingSq;
        Position blackKingSq;
        */
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
        // Bitboard hash;
        Bitboard squares_under_attack;
    };

    Bitboard set(Bitboard board, uint8 position);
    // Bitboard set(Bitboard board, Bitboard position);
    Bitboard clear(Bitboard board, uint8 position);
    // Bitboard clear(Bitboard board, Bitboard position);
    bool isSet(Bitboard board, uint8 position);
}
