#pragma once
#include "Engine.h"
#include <stack>

class UserInterface
{
public:
    void start();
    void startGameLoop(std::string fen = START_FEN);
    void startPerft(int depth, std::string fen = START_FEN, bool divide = true, uint64 expected = 0);
    void startUCI();
    void startSearch(int depth, std::string fen = START_FEN, Milliseconds time_limit = SEARCH_TIME_LIMIT);
    void parseParameters(int argc, char *argv[]);

private:
    Engine engine;
    Timer timer;
    std::stack<Move> move_history;

    // default parameters
    Mode mode = UCI_MODE;
    std::string start_fen = START_FEN;
    int depth = 0;
    bool divide = false;
    uint64 expected_perft = 0;
    Milliseconds search_time = SEARCH_TIME_LIMIT;

    void printHelp(std::string executable_name);
    Move getLegalMoveFromUser(MoveList legal_moves);
    void applyAndTrackMove(Move move);
    Piece getPromotionChoice();
};