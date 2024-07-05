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

    void initializePawnCaptureMasks();
    void initializeKnightMoves();
    void initializeBishopBlockers();
    void initializeRookBlockers();
    void initializeKingMoves();

    void addPawnMove(std::vector<Move> &moves, Move move, Board &board);
    void addCastlingMoves(std::vector<Move> &moves, Board &board);
    std::vector<Move> generateKingMoves(Board &board);
    std::vector<Move> generatePawnMoves(Board &board);
    std::vector<Move> generateKnightMoves(Board &board);
    std::vector<Move> generateBishopMoves(Board &board);
    std::vector<Move> generateRookMoves(Board &board);
    std::vector<Move> generateQueenMoves(Board &board);

    // precomputed attacks for sliding pieces (bishop and rook) using magic bitboards
    void initializeRookBishopAttacks();                                           // precomputes squares under attack for every position for every relevant occupancy
    Bitboard getOccupancyVariation(int index, int relevant_bits, Bitboard moves); // returns index-th occupancy variation of moves
    Bitboard precomputeSlidingRookAttacks(Position square, Bitboard occupied);
    Bitboard precomputeSlidingBishopAttacks(Position square, Bitboard occupied);

public:
    MoveGenerator();
    std::vector<Move> generateMoves(Board &board);
    bool squaresThreatened(Board &board, Bitboard squares, bool opponent);
};