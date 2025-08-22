#pragma once
#include "MoveGenerator.h"
#include "ZobristHash.h"
#include "TranspositionTable.h"
#include "Timer.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

class Engine
{
public:
    void initializeStartPosition(std::string fen);
    Move search();
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
    Board board;
    MoveGenerator move_generator;
    TranspositionTable transposition_table;
    ZobristHash zobrist;
    Timer timer;

    uint64 evaluated_nodes = 0;
    Score best_root_score = 0;
    Move best_root_move = NULL_MOVE;

    void calculateMoveScores(MoveList &moves);
    void sortMoves(MoveList &moves);
    Move pickBestMove(MoveList &moves);
    Score negamax_search(int depth, int remaining_depth, Score alpha, Score beta);
    Score quiescence(Score alpha, Score beta, int max_depth = MAX_QUIESCENCE_DEPTH);
};
