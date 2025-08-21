#pragma once
#include "MoveGenerator.h"
#include "ZobristHash.h"
#include "Timer.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

class Engine
{
public:
    void initializeStartPosition(std::string fen);
    Move search(int max_depth);
    uint64 perft(int depth, bool divide = false);
    Score evaluateBoard();
    void makeMove(Move move);
    void unmakeMove(Move move);

    // getter and setter
    GameState getGameState(MoveList moves);
    MoveList getLegalMoves();
    void setTimeLimit(Milliseconds time_limit);
    Board getBoard();

private:
    uint64 evaluated_nodes = 0;
    Move best_move = NULL_MOVE;
    Score best_score = 0;
    MoveGenerator move_generator;
    ZobristHash zobrist;
    Timer timer;
    Board board;

    void calculateMoveScores(MoveList &moves, Move best_move = NULL_MOVE);
    Move pickBestMove(MoveList &moves);
    Score negamax_search(int depth, Score alpha, Score beta, bool root = false);
    Score quiescence(Score alpha, Score beta, int max_depth = MAX_QUIESCENCE_DEPTH);
};
