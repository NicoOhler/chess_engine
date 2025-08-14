#pragma once
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

const uint64 PERFT_RESULTS[10] =
    {1, 20, 400, 8902, 197281, 4865609, 119060324, 3195901860, 84998978956, 2439530234167};

class Game
{
public:
    Game() = default;
    ~Game() = default;

    void startGame(std::string FEN = START_FEN);
    void applyMove(Board &board, Move move); // wrapper for move_generator.applyMove
    bool isGameOver(Board &board, MoveList moves);
    uint64 perft(int depth, std::string FEN = START_FEN, bool divide = true, uint64 expected = 0);

private:
    MoveGenerator move_generator;
    Move getLegalMoveFromUser(MoveList legal_moves);
    Piece getPromotionChoice();
    uint64 perft(int depth, Board board, bool divide = false);
};
