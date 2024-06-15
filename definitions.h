#pragma once

typedef unsigned long long uint64;
typedef uint64 Bitboard;
typedef unsigned char uint8;
typedef signed char int8;
typedef signed char Position;
typedef unsigned char Piece;

enum PIECES
{
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

/*
enum COLORS
{
    WHITE,
    BLACK
};
*/

enum ROWS
{
    ROW_1,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8
};

enum COLUMNS
{
    COL_A,
    COL_B,
    COL_C,
    COL_D,
    COL_E,
    COL_F,
    COL_G,
    COL_H
};
