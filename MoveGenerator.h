#pragma once
#include "definitions.h"
#include "BitBoard.h"
#include "Logger.h"
#include "Magics.h"
#include "utils.h"
#include <vector>

using namespace BitBoard;

struct Move
{
    Position from;
    Position to;
    Piece piece;
    Piece promotion;
    Bitboard castling;

    bool operator==(const Move &rhs)
    {
        return from == rhs.from && to == rhs.to && piece == rhs.piece; // piece is not necessary for move equality
    }
};

struct MoveList
{
    Move moves[MAX_MOVES];
    int size = 0;

    void append(Move move)
    {
        moves[size++] = move;
    }

    void clear()
    {
        size = 0;
    }

    bool empty()
    {
        return size == 0;
    }
};

class MoveGenerator
{
private:
    // attack masks
    Bitboard white_pawn_attack_right[NUM_SQUARES];
    Bitboard white_pawn_attack_left[NUM_SQUARES];
    Bitboard black_pawn_attack_right[NUM_SQUARES];
    Bitboard black_pawn_attack_left[NUM_SQUARES];
    Bitboard knight_moves[NUM_SQUARES];
    Bitboard queen_moves[NUM_SQUARES];
    Bitboard king_moves[NUM_SQUARES];

    // precomputed attacks for sliding pieces
    Bitboard bishop_blockers[NUM_SQUARES];
    Bitboard rook_blockers[NUM_SQUARES];
    Bitboard bishop_attacks[NUM_SQUARES][512];
    Bitboard rook_attacks[NUM_SQUARES][4096];

    // move generation masks
    void initializePawnCaptureMasks();
    void initializeKnightMoves();
    void initializeBishopBlockers();
    void initializeRookBlockers();
    void initializeKingMoves();

    // move generation
    MoveList generatePseudoLegalMoves(Board &board);
    void addPawnMoveWithPossiblePromotion(Board &board, MoveList &moves, Move move);
    void addCastlingMoves(Board &board, MoveList &moves);
    void generateKingMoves(Board &board, MoveList &moves);
    void generatePawnMoves(Board &board, MoveList &moves);
    void generateKnightMoves(Board &board, MoveList &moves);
    void generateBishopMoves(Board &board, MoveList &moves);
    void generateRookMoves(Board &board, MoveList &moves);
    void generateQueenMoves(Board &board, MoveList &moves);

    // precomputed attacks for sliding pieces (bishop and rook) using magic bitboards
    void initializeRookBishopAttacks();                                           // precomputes squares under attack for every position for every relevant occupancy
    Bitboard getOccupancyVariation(int index, int relevant_bits, Bitboard moves); // returns index-th occupancy variation of moves
    Bitboard precomputeSlidingRookAttacks(Position square, Bitboard occupied);
    Bitboard precomputeSlidingBishopAttacks(Position square, Bitboard occupied);

    // move application
    void handleCastling(Board &board, Move &move);
    void detectDoublePawnPushForEnPassant(Board &board, Move &move);

public:
    MoveGenerator();
    void applyMove(Board &board, Move move);
    MoveList generateLegalMoves(Board board);
    bool squaresThreatened(Board &board, Bitboard squares, bool opponent);
};