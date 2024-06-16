#pragma once
#include <string>
#define LOG_FILE "log.txt" // does not write to file if empty string
#define LOG_TO_CONSOLE true
#define APPEND_TO_FILE false

#define LOG_TYPE_ENABLED 0x1
#define WHITE 0x2

#define CHESS_BOARD (WHITE | LOG_TYPE_ENABLED)

#define MOVE_GENERATOR (WHITE | LOG_TYPE_ENABLED)

#define PAWN_MOVE (WHITE | LOG_TYPE_ENABLED)

#define KNIGHT_MOVE (WHITE | LOG_TYPE_ENABLED)

#define BISHOP_MOVE (WHITE | LOG_TYPE_ENABLED)

#define ROOK_MOVE (WHITE | LOG_TYPE_ENABLED)

#define QUEEN_MOVE (WHITE | LOG_TYPE_ENABLED)

#define KING_MOVE (WCHAR_MIN | LOG_TYPE_ENABLED)