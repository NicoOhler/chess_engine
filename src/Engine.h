#pragma once
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

class Engine
{
public:
    Engine() = default;
    ~Engine() = default;

    bool history_enabled = false;

    void startConsoleGame(std::string fen = START_FEN);
    uint64 startPerft(int depth, std::string fen = START_FEN, bool divide = true, uint64 expected = 0);
    void startUCI();
    void applyMove(Board &board, Move move);
    Board undoMove();
    GameState getGameState(Board &board, MoveList moves);
    void initializeGameHistory(Board board);
    void addBoardToHistory(Board board);
    void testSearch(std::string fen = START_FEN);
    Score search(Board board, int depth, Score alpha, Score beta);
    Score evaluateBoard(Board board);

private:
    Node *game_history = nullptr;
    MoveGenerator move_generator;
    Move getLegalMoveFromUser(MoveList legal_moves);
    Piece getPromotionChoice();
    uint64 perft(int depth, Board board, bool divide = false);
};
