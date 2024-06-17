#pragma once

typedef unsigned long long uint64;
typedef uint64 Bitboard;
typedef unsigned char uint8;
typedef signed char int8;
typedef signed char Position;
typedef Position Direction;
typedef unsigned char Piece;

// commonly used constants
const Position NUM_SQUARES = 64;
const Position NUM_ROWS = 8;
const Position NUM_COLS = 8;

// commonly used directions
const Position ONE_ROW_UP = 8;
const Position ONE_ROW_DOWN = -8;
const Position ONE_COL_LEFT = -1;
const Position ONE_COL_RIGHT = 1;
const Position ONE_ROW_UP_ONE_COL_LEFT = 7;
const Position ONE_ROW_UP_ONE_COL_RIGHT = 9;
const Position ONE_ROW_DOWN_ONE_COL_LEFT = -9;
const Position ONE_ROW_DOWN_ONE_COL_RIGHT = -7;

enum PIECES
{
    WHITE_PAWN = 'P',
    WHITE_KNIGHT = 'N',
    WHITE_BISHOP = 'B',
    WHITE_ROOK = 'R',
    WHITE_QUEEN = 'Q',
    WHITE_KING = 'K',
    BLACK_PAWN = 'p',
    BLACK_KNIGHT = 'n',
    BLACK_BISHOP = 'b',
    BLACK_ROOK = 'r',
    BLACK_QUEEN = 'q',
    BLACK_KING = 'k',
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
