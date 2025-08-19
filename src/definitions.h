#pragma once
#include <string>

typedef unsigned long long uint64;
typedef signed long Score;
typedef uint64 Bitboard;
typedef unsigned char uint8;
typedef signed char int8;
typedef signed char Position;
typedef Position Direction;
typedef unsigned char Piece;
typedef unsigned char Mode;
typedef unsigned char GameState;
typedef signed char Clock;
typedef uint64 Milliseconds;

// basic constants
const Position NUM_SQUARES = 64;
const Position NUM_ROWS = 8;
const Position NUM_COLS = 8;
const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const int MAX_MOVES = 200;
const Score POS_INFINITY = 2147483647;
const Score NEG_INFINITY = -2147483648;
const Clock HALF_MOVE_CLOCK_RESET = -1;
const Clock HALF_MOVE_CLOCK_LIMIT = 100;

// directions
const Position ONE_ROW_UP = 8;
const Position ONE_ROW_DOWN = -8;
const Position ONE_COL_LEFT = -1;
const Position ONE_COL_RIGHT = 1;
const Position ONE_ROW_UP_ONE_COL_LEFT = 7;
const Position ONE_ROW_UP_ONE_COL_RIGHT = 9;
const Position ONE_ROW_DOWN_ONE_COL_LEFT = -9;
const Position ONE_ROW_DOWN_ONE_COL_RIGHT = -7;

// engine
const Milliseconds SEARCH_TIME_LIMIT = 3000; // milliseconds
const Score PAWN_VALUE = 100;
const Score KNIGHT_VALUE = 300;
const Score BISHOP_VALUE = 330;
const Score ROOK_VALUE = 500;
const Score QUEEN_VALUE = 900;
const Score KING_VALUE = 20000;

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
    EMPTY = ' ',
    UNDO = 'u'
};

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

enum MODES
{
    M_UCI = 'u',
    M_CONSOLE = 'c',
    M_PERFT = 'p',
    M_SEARCH = 's'
};

enum GAME_STATE
{
    IN_PROGRESS,
    CHECKMATE,
    DRAW // equivalent to stalemate
};