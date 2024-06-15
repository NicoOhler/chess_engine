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
    Move getLegalMoveFromUser();
    bool isLegalMove(Move move);
    void applyMove(Move move);
    void printBoard();
    bool isGameOver();
    bool whiteTurn();
    Bitboard getBitboardByPiece(char piece);

    // ? private:
    MoveGenerator moveGenerator;
    Board board;
    char printable_board[8][8] = { // needs to be updated after each move
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};
};
