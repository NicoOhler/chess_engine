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
    // todo void initStartBoard(std::string fen = START_FEN);
    char board_to_print[8][8];

    Move getLegalMoveFromUser(std::vector<Move> legal_moves);
    Piece getPromotionChoice();
    void applyMove(Move move);
    bool isGameOver();
    Bitboard *getBitboardByPiece(char piece);
    void printBoard();
    void placePiecesOnBoard();
    void placePieceTypeOnBoard(Bitboard piece_type, char piece);
};
