#pragma once
#include "MoveGenerator.h"
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
    void startSearch(int depth, std::string fen = START_FEN);
    GameState getGameState(Board &board, MoveList moves);
    void iterativeDeepening(Board &board, int max_depth);
    Score evaluateBoard(Board board);

private:
    uint64 counter = 0;
    Move best_move = NULL_MOVE;
    Score best_score = 0;
    std::stack<Move> move_history;
    MoveGenerator move_generator;

    void orderMoves(MoveList &moves, Board &board);
    Score search(Board board, int depth, Score alpha, Score beta);
    Move getLegalMoveFromUser(MoveList legal_moves);
    void applyAndTrackMove(Board &board, Move move);
    Piece getPromotionChoice();
    uint64 perft(int depth, Board board, bool divide = false);
};
