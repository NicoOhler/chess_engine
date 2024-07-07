#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

#define whites_turn board.white_to_move

class Game
{
public:
    Game(std::string FEN = START_FEN);
    ~Game() = default;

    void startGame();

private:
    Board board;
    MoveGenerator moveGenerator;
    char board_to_print[8][8];

    Move getLegalMoveFromUser(MoveList legal_moves);
    Piece getPromotionChoice();

    bool isGameOver(Board &board, MoveList moves);
    void applyMove(Board &board, Move move);
    void applyPromotion(Board &board, Move &move);
    void applyCastling(Board &board, Move &move);
    void updateCastlingRights(Board &board, Move &move);
    void applyEnPassantCapture(Board &board, Move &move);
    void detectDoublePawnPushForEnPassant(Board &board, Move &move);
    MoveList generateLegalMoves(Board board);

    // todo move to separate class
    void printBoard();
    void placePiecesOnBoard();
    void placePieceTypeOnBoard(Bitboard piece_type, char piece);
};
