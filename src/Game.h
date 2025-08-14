#pragma once
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

class Game
{
public:
    Game() = default;
    ~Game() = default;

    bool history_enabled = true;

    void startGame(std::string FEN = START_FEN);
    void applyMove(Board &board, Move move);
    Board undoMove();
    bool isGameOver(Board &board, MoveList moves);
    uint64 perft(int depth, std::string FEN = START_FEN, bool divide = true, uint64 expected = 0);
    void initializeGameHistory(Board board);
    void addBoardToHistory(Board board);

private:
    Node *game_history = nullptr;
    MoveGenerator move_generator;
    Move getLegalMoveFromUser(MoveList legal_moves);
    Piece getPromotionChoice();
    uint64 perft(int depth, Board board, bool divide = false);
};
