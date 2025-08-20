#pragma once
#include "MoveGenerator.h"
#include "Timer.h"
#include "definitions.h"
#include <iostream>
#include <stack>

using namespace BitBoard;
using namespace std;

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    void startConsoleGame(std::string fen = START_FEN);
    uint64 startPerft(int depth, std::string fen = START_FEN, bool divide = true, uint64 expected = 0);
    void startUCI();
    void startSearch(int depth, std::string fen = START_FEN, Milliseconds time_limit = SEARCH_TIME_LIMIT);
    GameState getGameState(MoveList moves);
    void iterativeDeepening(int max_depth);
    Score evaluateBoard();

private:
    uint64 counter = 0;
    Move best_move = NULL_MOVE;
    Score best_score = 0;
    std::stack<Move> move_history;
    MoveGenerator move_generator;
    Timer timer;
    Board board;

    void calculateMoveScores(MoveList &moves, Move best_move = NULL_MOVE);
    Move pickBestMove(MoveList &moves);
    Score search(int depth, Score alpha, Score beta, bool root = false);
    Score quiescence(Score alpha, Score beta, int max_depth = MAX_QUIESCENCE_DEPTH);
    Move getLegalMoveFromUser(MoveList legal_moves);
    void applyAndTrackMove(Move move);
    Piece getPromotionChoice();
    uint64 perft(int depth, bool divide = false);
};
