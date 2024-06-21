#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

#define white_to_move board.white_to_move
#define black_to_move !white_to_move

class Game
{
public:
    Game() = default;
    ~Game() = default;

    void loop();

private:
    MoveGenerator moveGenerator;
    Board board; // game state
    char board_to_print[8][8];
    // todo void initStartBoard(std::string fen = START_FEN);

    Move getLegalMoveFromUser(std::vector<Move> legal_moves);
    bool isGameOver();
    Bitboard *getBitboardByPiece(char piece);

    void applyMove(Move move);
    Piece getPromotionChoice();
    void applyPromotion(Move &move);
    void applyCastling(Move &move);
    void updateCastlingRights(Move &move);
    void applyEnPassantCapture(Move &move);
    void detectDoublePawnPushForEnPassant(Move &move);

    // todo move to separate class
    void printBoard();
    void placePiecesOnBoard();
    void placePieceTypeOnBoard(Bitboard piece_type, char piece);
};
