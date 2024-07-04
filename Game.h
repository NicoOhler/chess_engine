#pragma once
#include "BitBoard.h"
#include "MoveGenerator.h"
#include "definitions.h"
#include <iostream>

using namespace BitBoard;
using namespace std;

#define white_to_move board.white_to_move

class Game
{
public:
    Game() = default;
    ~Game() = default;

    void loop();

private:
    Board board;
    MoveGenerator moveGenerator;
    char board_to_print[8][8];
    // todo void initStartBoard(std::string fen = START_FEN);

    Move getLegalMoveFromUser(std::vector<Move> legal_moves);
    Piece getPromotionChoice();

    bool isGameOver();
    void applyMove(Board &board, Move move);
    void applyPromotion(Board &board, Move &move);
    void applyCastling(Board &board, Move &move);
    void updateCastlingRights(Board &board, Move &move);
    void applyEnPassantCapture(Board &board, Move &move);
    void detectDoublePawnPushForEnPassant(Board &board, Move &move);
    std::vector<Move> removeIllegalMoves(std::vector<Move> moves, Board board);

    // todo move to separate class
    void printBoard();
    void placePiecesOnBoard();
    void placePieceTypeOnBoard(Bitboard piece_type, char piece);
};
