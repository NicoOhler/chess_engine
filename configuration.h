#pragma once
#include <string>

typedef int LogType;

const std::string LOG_FILE = "log.txt"; // does not write to file if empty string
const bool LOG_TO_CONSOLE = true;
const bool APPEND_TO_FILE = false;

const LogType LOG_TYPE_ENABLED = 0x1;
const LogType WHITE = 0x2; // todo implement color logging

const LogType CHESS_BOARD = WHITE | LOG_TYPE_ENABLED;
const LogType MOVE_GENERATOR = WHITE | LOG_TYPE_ENABLED;
const LogType PAWN_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType KNIGHT_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType BISHOP_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType ROOK_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType QUEEN_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType KING_MOVE = WHITE | LOG_TYPE_ENABLED;
const LogType REMOVE_ILLEGAL_MOVES = WHITE | LOG_TYPE_ENABLED;
