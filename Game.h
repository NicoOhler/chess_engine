#pragma once
#include "BitBoard.h"
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

    void loop();

private:
    MoveGenerator moveGenerator;
    Board board;
    // todo fill with FEN
    char printable_board[8][8] = { // needs to be updated after each move
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

    Move getLegalMoveFromUser(std::vector<Move> legal_moves);
    void applyMove(Move move);
    void printBoard();
    bool isGameOver();
    Bitboard getBitboardByPiece(char piece);
};
