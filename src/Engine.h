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
    GameState getGameState(Board &board, MoveList moves);
    void testSearch(std::string fen = START_FEN);
    Score search(Board board, int depth, Score alpha, Score beta);
    Score evaluateBoard(Board board);

private:
    std::stack<Move> move_history;
    MoveGenerator move_generator;
    Move getLegalMoveFromUser(MoveList legal_moves);
    void applyAndTrackMove(Board &board, Move move);
    Piece getPromotionChoice();
    uint64 perft(int depth, Board board, bool divide = false);
};
