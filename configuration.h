#pragma once
#include <string>
#define LOG_FILE "log.txt" // does not write to file if empty string
#define LOG_TO_CONSOLE true
#define APPEND_TO_FILE true

struct LogType
{
    std::string name;
    bool active;
    int color; // todo implement colors
};

#define CHESS_BOARD \
    LogType { "CHESS_BOARD", true, 0 }

#define MOVE_GENERATOR \
    LogType { "MOVE_GENERATOR", true, 0 }